# *************************************
# Pandora Storage Server Python Client
# Client
# 'pandora_client.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

import requests

class ResponseObject:

    def __init__(self, http_status_code: str, http_response_body: str, error: bool = False, pandora_error_code: int = 0, pandora_error_message: str = '') -> None:

        self.http_status_code = http_status_code
        self.http_response_body = http_response_body
        self.error = error
        self.pandora_error_code = pandora_error_code
        self.pandora_error_message = pandora_error_message

class PandoraClient:

    # Default Server IP Address
    LOCALHOST_ADDRESS = '127.0.0.1'
    #Default Port Number
    DEFAULT_PORT = '5000'
    # Protocols
    HTTP = 'http'
    HTTPS = 'https'
    # Constants
    PANDORA = 'pandora'
    CREATE_ELEMENT_CONTAINER = 'create-element-container'
    INTERNAL_ERROR_CODE = 500
    ERROR_CODE_LENGTH = 3
    ERROR_MESSAGE_START_INDEX = 31

    def __init__(self, server_ip_address: str = None, server_port_number: str = None, throw_exceptions: bool = True) -> None:

        self.server_ip_address = self.LOCALHOST_ADDRESS if server_ip_address is None else server_ip_address
        self.server_port_number = self.DEFAULT_PORT if server_port_number is None else server_port_number
        self.throw_exceptions = throw_exceptions

    def create_element_container(self, element_container_name: str) -> ResponseObject:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.CREATE_ELEMENT_CONTAINER, element_container_name)
        request_response = requests.put(request_endpoint)
        
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            if(self.throw_exceptions): raise Exception(request_response.text) 
            return ResponseObject(request_response.status_code, request_response.text, error=True, pandora_error_code=int(request_response.text[0:self.ERROR_CODE_LENGTH]), pandora_error_message=request_response.text[self.ERROR_MESSAGE_START_INDEX:])

        return ResponseObject(request_response.status_code, request_response.text)