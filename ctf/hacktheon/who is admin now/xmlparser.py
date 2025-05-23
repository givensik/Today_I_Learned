import xml.etree.ElementTree as ET

xml_string = """<?xml version="1.0" encoding="UTF-8"?>
<user>
    <user_id>admin@test</user_id>
    <username>admin</username>
    <role>admin</role>
</user>"""

root = ET.fromstring(xml_string)
print(root.tag)  # 'user'가 출력되면 성공