from lxml import etree
import sys

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
print(result_tree)
