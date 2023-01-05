# *************************************
# Pandora Storage Server Python Client
# Examples
# 'get_element.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

import pandora_storage_server_client as pandora

def main():

    pandora_client = pandora.PandoraClient()

    school_grades_element_container = 'SchoolGrades'
    student_name = 'Maria'
    student_grade = '100'

    pandora_client.create_element_container(element_container_name=school_grades_element_container)

    pandora_client.set_element(element_container_name=school_grades_element_container, element_id=student_name, element_value=student_grade)

    query = pandora_client.get_element(element_container_name=school_grades_element_container, element_id=student_name)

    if(query.error):
        print('Error Code: {0} | [{1}] Server Ellapsed Time: {2} ms | {3}'.format(
              query.error_code, query.transaction_code, query.server_ellapsed_time, query.response))
    else:
        print('{0}'.format(query.result))

if __name__ == '__main__':
    main()