# *************************************
# Pandora Storage Server Python Client
# Examples
# 'create_element_container.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

import pandora_storage_server_client as pandora

def main():

    pandora_client = pandora.PandoraClient()

    school_grades_element_container = 'SchoolGrades'

    query = pandora_client.create_element_container(element_container_name=school_grades_element_container)

    if(query.error):
        print('HTTP {0} | Response Result is: {1} | Error Code: {2} | [{3}] Server Ellapsed Time: {4} ms | {5}'.format(
              query.http_status_code, query.result, query.error_code, query.transaction_code, query.server_ellapsed_time, query.response))
    else:
        print('HTTP {0} | Response Result is: {1} | [{2}] Server Ellapsed Time: {3} ms | {4}'.format(
              query.http_status_code, query.result, query.transaction_code, query.server_ellapsed_time, query.response))

if __name__ == '__main__':
    main()