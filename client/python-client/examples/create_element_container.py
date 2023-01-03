# *************************************
# Pandora Storage Server Python Client
# Examples
# 'create_element_container.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

pandora_client = PandoraClient(throw_exceptions=False)

response = pandora_client.create_element_container('MyElementContainer')

if(response.error):
    print('Response had error code: {0} | {1}'.format(response.pandora_error_code, response.pandora_error_message))
else:
    print('Succesful operation: {0}'.format(response.http_response_body))