from lxml import etree
import sys
import subprocess
import logging

logger = logging.getLogger("xml_convert")
logger.setLevel(logging.INFO)
formatter = logging.Formatter('[%(asctime)s][%(levelname)s] %(message)s')
handler = logging.StreamHandler()
handler.setFormatter(formatter)
handler.setLevel(logging.INFO)
logger.addHandler(handler)

def get_file_path(_suite_name, _case_name):
    space = "[ \\t]*"
    case_pattern = "^{space}(TEST|TEST_F){space}\({space}{suite_name}{space},{space}{case_name}{space}\)"\
        .format(space=space, suite_name=_suite_name, case_name=_case_name)
    cmd = 'grep -E -r -l "{0}"'.format(case_pattern)
    #logger.info(cmd)
    process = subprocess.Popen(cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    result = ""
    while process.poll() is None:
        line = process.stdout.readline()
        result += line.strip()

    return result

def main():
    xml_input = sys.argv[1] # build/gtestresults.xml
    xsl_templ = sys.argv[2] # sonar_test.xsl

    xmldoc = None
    transform = None
    with open(xml_input) as f:
        xmldoc = etree.parse(f)

    with open(xsl_templ) as xsltfile:
        xslt_root = etree.XML(xsltfile.read())
        transform = etree.XSLT(xslt_root)

    result_tree = transform(xmldoc)
    #print(result_tree)

    context = etree.iterwalk(result_tree, tag="file")
    for action, elem in context:
        testcase = elem[0]
        classname = testcase.attrib["classname"]
        casename = testcase.attrib["name"]
        real_file_path = get_file_path(classname, casename)
        if len(real_file_path) > 0:
            elem.attrib["path"] = real_file_path

    print(result_tree)

if __name__ == "__main__":
    main()
