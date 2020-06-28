#ifndef __UNITTEST_STUB_H__
#define __UNITTEST_STUB_H__

//https://github.com/coolxv/cpp-stub

#ifdef _WIN32
#include <windows.h>
#else
//linux
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>
#endif
//c
#include <cstdio>
#include <cstdlib>
#include <climits>
//c++
#include <iostream>
#include <exception>
#include <map>
#include <alloca.h>
#include <stdexcept>

#ifndef _WIN32
//https://stackoverflow.com/questions/2152958/reading-memory-protection
//https://github.com/18446744073709551615/reDroid/blob/master/jni/reDroid/re_mprot.c
#define __OR_RSHIFT__(n,x) ((x)|(x)>>n)
#define FIRST_UNUSED_BIT(x) (1+__OR_RSHIFT__(16,__OR_RSHIFT__(8,__OR_RSHIFT__(4,__OR_RSHIFT__(2,__OR_RSHIFT__(1,x))))))
enum {
    MPROT_0 = 0, // not found at all
    MPROT_R = PROT_READ,                                 // readable
    MPROT_W = PROT_WRITE,                                // writable
    MPROT_X = PROT_EXEC,                                 // executable
    MPROT_S = FIRST_UNUSED_BIT(MPROT_R|MPROT_W|MPROT_X), // shared
    MPROT_P = MPROT_S<<1,                                // private
};

struct buffer {
    int pos;
    int size;
    char* mem;
};

static char* _buf_reset(struct buffer*b) {
    b->mem[b->pos] = 0;
    b->pos = 0;
    return b->mem;
}

static struct buffer* _new_buffer(int length) {
    struct buffer* res = (struct buffer *)malloc(sizeof(struct buffer)+length+8);
    res->pos = 0;
    res->size = length;
    res->mem = (char*)(res+1);
    return res;
}

static int _buf_putchar(struct buffer*b, int c) {
    b->mem[b->pos++] = c;
    return b->pos >= b->size;
}

static unsigned int read_mprotection(void* addr) {
    int a;
    unsigned int res = MPROT_0;
    FILE *f = fopen("/proc/self/maps", "r");
    struct buffer* b = _new_buffer(1024);
    while ((a = fgetc(f)) >= 0) {
        if (_buf_putchar(b,a) || a == '\n') {
            char*end0 = (char*)0;
            unsigned long addr0 = strtoul(b->mem, &end0, 0x10);
            char*end1 = (char*)0;
            unsigned long addr1 = strtoul(end0+1, &end1, 0x10);
            if ((void*)addr0 < addr && addr < (void*)addr1) {
                res |= (end1+1)[0] == 'r' ? MPROT_R : 0;
                res |= (end1+1)[1] == 'w' ? MPROT_W : 0;
                res |= (end1+1)[2] == 'x' ? MPROT_X : 0;
                res |= (end1+1)[3] == 'p' ? MPROT_P
                     : (end1+1)[3] == 's' ? MPROT_S : 0;
                break;
            }
            _buf_reset(b);
        }
    }
    free(b);
    fclose(f);
    return res;
}

static char* _mprot_tostring_(char*buf, unsigned int prot) {
    buf[0] = (prot & MPROT_R) ? 'r' : '-';
    buf[1] = (prot & MPROT_W) ? 'w' : '-';
    buf[2] = (prot & MPROT_X) ? 'x' : '-';
    buf[3] = (prot & MPROT_S) ? 's' : (prot & MPROT_P) ? 'p' :  '-';
    buf[4] = 0;
    return buf;
}
// convert the protection mask into a string. Uses alloca(), no need to free() the memory!
#define mprot_tostring(x) ( _mprot_tostring_( (char*)alloca(8) , (x) ) )

static int mprot_std(unsigned int mprot) {
    int prot = 0;
    prot |= (mprot & MPROT_R) ? PROT_READ : 0;
    prot |= (mprot & MPROT_W) ? PROT_WRITE : 0;
    prot |= (mprot & MPROT_X) ? PROT_EXEC : 0;
    return prot;
}
#endif

//base on C++03
/**********************************************************
                  replace function
**********************************************************/
#define ADDR(CLASS_NAME,MEMBER_NAME) (&CLASS_NAME::MEMBER_NAME)
#ifdef __x86_64__
#define CODESIZE 13U
#define CODESIZE_MIN 5U
#define CODESIZE_MAX CODESIZE
#else
#define CODESIZE 5U
#endif

struct func_stub
{
    void *fn;
    unsigned char code_buf[CODESIZE];
    bool far_jmp;
};

class Stub
{
public:
    Stub()
    {
#ifdef _WIN32
        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);
        m_pagesize = sys_info.dwPageSize;
#else
        m_pagesize = sysconf(_SC_PAGE_SIZE);
#endif

        if (m_pagesize < 0)
        {
            m_pagesize = 4096;
        }
    }

    ~Stub()
    {
        std::map<void*,func_stub*>::iterator iter;
        for (iter = m_result.begin(); iter != m_result.end(); ++iter)
        {
            struct func_stub *pstub = iter->second;
#ifdef _WIN32
            int page_count = get_page_count(pstub);
            DWORD lpflOldProtect;
            if (0 == VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READWRITE, &lpflOldProtect))
#else
            int old_protect = set_mprotect(pstub);
#endif
            {
#ifdef __x86_64__
                if (pstub->far_jmp)
                {
                    memcpy(pstub->fn, pstub->code_buf, CODESIZE_MAX);
                }
                else
                {
                    memcpy(pstub->fn, pstub->code_buf, CODESIZE_MIN);
                }
#else
                memcpy(pstub->fn, pstub->code_buf, CODESIZE);
#endif
#ifdef _WIN32
                VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READ, &lpflOldProtect);
#else
                restore_mprotect(pstub, old_protect);
#endif
            }

            iter->second = NULL;
            delete pstub;
        }

        return;
    }

    template<typename T,typename S>
    void set(T addr, S addr_stub)
    {
        void * fn;
        void * fn_stub;
        fn = addrof(addr);
        fn_stub = addrof(addr_stub);
        struct func_stub *pstub;
        pstub = new func_stub;
        m_result.insert(std::pair<void*, func_stub*>(fn,pstub));
        //start
        pstub->fn = fn;
#ifdef __x86_64__
        if (distanceof(fn, fn_stub))
        {
            pstub->far_jmp = true;
            memcpy(pstub->code_buf, fn, CODESIZE_MAX);
        }
        else
        {
            pstub->far_jmp = false;
            memcpy(pstub->code_buf, fn, CODESIZE_MIN);
        }
#else
        memcpy(pstub->code_buf, fn, CODESIZE);
#endif
#ifdef _WIN32
        int page_count = get_page_count(pstub);
        DWORD lpflOldProtect;
        if (0 == VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READWRITE, &lpflOldProtect))
        {
            throw("stub set mprotect to w+r+x faild");
        }
#else
        int old_protect = set_mprotect(pstub);
#endif
#ifdef __x86_64__
        if (pstub->far_jmp)
        {
            //13 byte
            *(unsigned char*)fn = 0x49;
            *((unsigned char*)fn + 1) = 0xbb;
            *(unsigned long long *)((unsigned char *)fn + 2) = (unsigned long long)fn_stub;
            *(unsigned char *)((unsigned char *)fn + 10) = 0x41;
            *(unsigned char *)((unsigned char *)fn + 11) = 0x53;
            *(unsigned char *)((unsigned char *)fn + 12) = 0xc3;
        }
        else
        {
            //5 byte
            *(unsigned char *)fn = (unsigned char)0xE9; // asm jmp
            *(unsigned int *)((unsigned char *)fn + 1) = (unsigned char *)fn_stub - (unsigned char *)fn - CODESIZE_MIN;
        }
#else
         //5 byte
        *(unsigned char *)fn = (unsigned char)0xE9;
        *(unsigned int *)((unsigned char *)fn + 1) = (unsigned char *)fn_stub - (unsigned char *)fn - CODESIZE;
#endif

#ifdef _WIN32
        if (0 == VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READ, &lpflOldProtect))
        {
            throw("stub set mprotect to r+x failed");
        }
#else
        restore_mprotect(pstub, old_protect);
#endif
        return;
    }

    template<typename T>
    void reset(T addr)
    {
        void * fn;
        fn = addrof(addr);

        std::map<void*,func_stub*>::iterator iter = m_result.find(fn);

        if (iter == m_result.end())
        {
            return;
        }
        struct func_stub *pstub;
        pstub = iter->second;

#ifdef _WIN32
        int page_count = get_page_count(pstub);
        DWORD lpflOldProtect;
        if (0 == VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READWRITE, &lpflOldProtect))
        {
            throw("stub reset mprotect to w+r+x faild");
        }
#else
        int old_protect = set_mprotect(pstub);
#endif
#ifdef __x86_64__
        if (pstub->far_jmp)
        {
            memcpy(pstub->fn, pstub->code_buf, CODESIZE_MAX);
        }
        else
        {
            memcpy(pstub->fn, pstub->code_buf, CODESIZE_MIN);
        }
#else
        memcpy(pstub->fn, pstub->code_buf, CODESIZE);
#endif

#ifdef _WIN32
        if (0 == VirtualProtect(pageof(pstub->fn), m_pagesize * page_count, PAGE_EXECUTE_READ, &lpflOldProtect))
        {
            throw("stub reset mprotect to r+x failed");
        }
#else
        restore_mprotect(pstub, old_protect);
#endif
        m_result.erase(iter);
        delete pstub;

        return;
    }

private:
    void *pageof(const void* p)
    {
#ifdef _WIN32
        return (void *)((unsigned long long)p & ~(m_pagesize - 1));
#else
        return (void *)((unsigned long)p & ~(m_pagesize - 1));
#endif
    }

    int get_page_count(const struct func_stub *pstub)
    {
#ifdef _WIN32
        return 2;
#else
        // maybe code would cross two pages!
        char *code_start = (char *)pstub->fn;
#ifdef __x86_64__
        int code_size = (pstub->far_jmp ? CODESIZE_MAX : CODESIZE_MIN);
#else
        int code_size = CODESIZE;
#endif
        char *code_end = code_start + code_size - 1;
        void *page1 = pageof(code_start);
        void *page2 = pageof(code_end);
        int count = (page1 == page2 ? 1 : 2);
        return count;
#endif
    }

#ifndef _WIN32
    int set_mprotect(const struct func_stub *pstub)
    {
        int page_count = get_page_count(pstub);
        if (page_count != 1)
        {
            std::string what_err("stub cross page!");
            throw std::logic_error(what_err);
        }
        unsigned int mprot = read_mprotection(pstub->fn);
        int prot = 0;
        if ((mprot & MPROT_W) == 0)
        {
            prot = mprot_std(mprot);
            if (-1 == mprotect(pageof(pstub->fn), m_pagesize * page_count, prot | PROT_WRITE))
            {
                std::string what_err("stub set mprotect to w+r+x faild");
                throw std::logic_error(what_err);
            }
        }
        return prot;
    }

    void restore_mprotect(const struct func_stub *pstub, int prot)
    {
        if (prot == 0) {
            return;
        }
        int page_count = get_page_count(pstub);
        if (-1 == mprotect(pageof(pstub->fn), m_pagesize * page_count, prot))
        {
            std::string what_err("stub restore mprotect failed");
            throw std::logic_error(what_err);
        }
    }
#endif

    template<typename T>
    void* addrof(T src)
    {
        union
        {
          T _s;
          void* _d;
        }ut;
        ut._s = src;
        return ut._d;
    }

#ifdef __x86_64__
    bool distanceof(void* addr, void* addr_stub)
    {
        unsigned long long addr_tmp = (unsigned long long)addr;
        unsigned long long addr_stub_tmp = (unsigned long long)addr_stub;
        unsigned int int_addr_tmp = (unsigned int)(addr_tmp >> 32);
        unsigned int int_addr_stub_tmp = (unsigned int)(addr_stub_tmp >> 32);
        if ((int_addr_tmp > 0 && int_addr_stub_tmp > 0) || (int_addr_tmp == 0 && int_addr_stub_tmp == 0))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
#endif

private:
#ifdef _WIN32
    long long m_pagesize;
#else
    long m_pagesize;
#endif
    std::map<void*, func_stub*> m_result;
};

#endif

