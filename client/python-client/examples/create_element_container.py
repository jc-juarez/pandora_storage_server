# *************************************
# Pandora Storage Server Python Client
# Examples
# 'create_element_container.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

import pandora_storage_server_client as pandora

def main():

    pandora_client = pandora.PandoraClient(throw_exceptions=False)

    query = pandora_client.create_element_container('MyElementContainer')

    if(query.error):
        print('Response had error code: {0} | {1}'.format(query.pandora_error_code, query.pandora_error_message))
    else:
        print('Succesful operation: HTTP {0} | {1}'.format(query.http_status_code, query.response))

if __name__ == '__main__':
    main()
