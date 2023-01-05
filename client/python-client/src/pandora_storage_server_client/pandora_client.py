# *************************************
# Pandora Storage Server Python Client
# Client
# 'pandora_client.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

import requests

def get_between(text: str, first: str, last: str) -> str:
    
    try:
        start_index = text.index(first) + len(first)
        end_index = text.index(last, start_index)
        return text[start_index:end_index]
    except ValueError:
        return ""

class PandoraResponse:

    def __init__(self, http_status_code: str, response: str, result: str, transaction_code: str = '', server_ellapsed_time: float = 0.0, error: bool = False, error_code: int = 0) -> None:

        self.http_status_code = http_status_code
        self.response = response
        self.result = result
        self.transaction_code = transaction_code
        self.server_ellapsed_time = server_ellapsed_time
        self.error = error
        self.error_code = error_code

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
    DELETE_ELEMENT_CONTAINER = 'delete-element-container'
    SET_ELEMENT = 'set-element'
    GET_ELEMENT = 'get-element'
    DELETE_ELEMENT = 'delete-element'
    OK_CODE = 200
    INTERNAL_ERROR_CODE = 500
    UNKNOWN_PANDORA_ERROR_CODE = 444
    SUCCESFUL_RESULT = 'success'
    FAILED_RESULT = 'failed'
    RESPONSE_SEPARATION_LENGTH = 2
    TRANSACTION_CODE_START_CHARACTER = '['
    TRANSACTION_CODE_END_CHARACTER = ']'
    SERVER_ELLAPSED_TIME_START_CHARACTER = '{'
    SERVER_ELLAPSED_TIME_END_CHARACTER = '}'
    ERROR_CODE_START_CHARACTER = '<'
    ERROR_CODE_END_CHARACTER = '>'
    NOT_FOUND_STRING = ''

    def __init__(self, server_ip_address: str = None, server_port_number: str = None, throw_exceptions: bool = False) -> None:

        self.server_ip_address = self.LOCALHOST_ADDRESS if server_ip_address is None else server_ip_address
        self.server_port_number = self.DEFAULT_PORT if server_port_number is None else server_port_number
        self.throw_exceptions = throw_exceptions

    def create_element_container(self, element_container_name: str) -> PandoraResponse:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.CREATE_ELEMENT_CONTAINER, element_container_name)
        request_response = requests.put(request_endpoint)

        # Throw fast exception
        if(request_response.status_code != self.OK_CODE and self.throw_exceptions): 
            raise Exception(request_response.text)

        # Not Valid Request or Unknown Error
        if(request_response.status_code != self.OK_CODE and request_response.status_code != self.INTERNAL_ERROR_CODE):
            return PandoraResponse(http_status_code=request_response.status_code, response=request_response.text, result=self.FAILED_RESULT, error=True, error_code=self.UNKNOWN_PANDORA_ERROR_CODE)

        # General Request Response properties
        response = request_response.text[request_response.text.index('|')+self.RESPONSE_SEPARATION_LENGTH:]
        transaction_code = get_between(request_response.text, self.TRANSACTION_CODE_START_CHARACTER, self.TRANSACTION_CODE_END_CHARACTER)
        server_ellapsed_time = get_between(request_response.text, self.SERVER_ELLAPSED_TIME_START_CHARACTER, self.SERVER_ELLAPSED_TIME_END_CHARACTER)

        # Pandora Internal Error
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            error_code = get_between(request_response.text, self.ERROR_CODE_START_CHARACTER, self.ERROR_CODE_END_CHARACTER)
            return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.FAILED_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time), error=True, error_code=int(error_code))

        # Succesful Pandora Transaction
        return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.SUCCESFUL_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time))

    def remove_element_container(self, element_container_name: str) -> PandoraResponse:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.DELETE_ELEMENT_CONTAINER, element_container_name)
        request_response = requests.delete(request_endpoint)
        
        # Throw fast exception
        if(request_response.status_code != self.OK_CODE and self.throw_exceptions): 
            raise Exception(request_response.text)

        # Not Valid Request or Unknown Error
        if(request_response.status_code != self.OK_CODE and request_response.status_code != self.INTERNAL_ERROR_CODE):
            return PandoraResponse(http_status_code=request_response.status_code, response=request_response.text, result=self.FAILED_RESULT, error=True, error_code=self.UNKNOWN_PANDORA_ERROR_CODE)

        # General Request Response properties
        response = request_response.text[request_response.text.index('|')+self.RESPONSE_SEPARATION_LENGTH:]
        transaction_code = get_between(request_response.text, self.TRANSACTION_CODE_START_CHARACTER, self.TRANSACTION_CODE_END_CHARACTER)
        server_ellapsed_time = get_between(request_response.text, self.SERVER_ELLAPSED_TIME_START_CHARACTER, self.SERVER_ELLAPSED_TIME_END_CHARACTER)

        # Pandora Internal Error
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            error_code = get_between(request_response.text, self.ERROR_CODE_START_CHARACTER, self.ERROR_CODE_END_CHARACTER)
            return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.FAILED_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time), error=True, error_code=int(error_code))

        # Succesful Pandora Transaction
        return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.SUCCESFUL_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time))

    def set_element(self, element_container_name: str, element_id: str, element_value: str) -> PandoraResponse:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}/{6}/{7}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.SET_ELEMENT, element_container_name, element_id, element_value)
        request_response = requests.post(request_endpoint)

        # Throw fast exception
        if(request_response.status_code != self.OK_CODE and self.throw_exceptions): 
            raise Exception(request_response.text)

        # Not Valid Request or Unknown Error
        if(request_response.status_code != self.OK_CODE and request_response.status_code != self.INTERNAL_ERROR_CODE):
            return PandoraResponse(http_status_code=request_response.status_code, response=request_response.text, result=self.FAILED_RESULT, error=True, error_code=self.UNKNOWN_PANDORA_ERROR_CODE)

        # General Request Response properties
        response = request_response.text[request_response.text.index('|')+self.RESPONSE_SEPARATION_LENGTH:]
        transaction_code = get_between(request_response.text, self.TRANSACTION_CODE_START_CHARACTER, self.TRANSACTION_CODE_END_CHARACTER)
        server_ellapsed_time = get_between(request_response.text, self.SERVER_ELLAPSED_TIME_START_CHARACTER, self.SERVER_ELLAPSED_TIME_END_CHARACTER)

        # Pandora Internal Error
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            error_code = get_between(request_response.text, self.ERROR_CODE_START_CHARACTER, self.ERROR_CODE_END_CHARACTER)
            return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.FAILED_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time), error=True, error_code=int(error_code))

        # Succesful Pandora Transaction
        return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.SUCCESFUL_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time))

    def get_element(self, element_container_name: str, element_id: str) -> PandoraResponse:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}/{6}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.GET_ELEMENT, element_container_name, element_id)
        request_response = requests.get(request_endpoint)

        # Throw fast exception
        if(request_response.status_code != self.OK_CODE and self.throw_exceptions): 
            raise Exception(request_response.text)

        # Not Valid Request or Unknown Error
        if(request_response.status_code != self.OK_CODE and request_response.status_code != self.INTERNAL_ERROR_CODE):
            return PandoraResponse(http_status_code=request_response.status_code, response=request_response.text, result=self.NOT_FOUND_STRING, error=True, error_code=self.UNKNOWN_PANDORA_ERROR_CODE)

        # General Request Response properties
        response = request_response.text[request_response.text.index('|')+self.RESPONSE_SEPARATION_LENGTH:]
        transaction_code = get_between(request_response.text, self.TRANSACTION_CODE_START_CHARACTER, self.TRANSACTION_CODE_END_CHARACTER)
        server_ellapsed_time = get_between(request_response.text, self.SERVER_ELLAPSED_TIME_START_CHARACTER, self.SERVER_ELLAPSED_TIME_END_CHARACTER)

        # Pandora Internal Error
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            error_code = get_between(request_response.text, self.ERROR_CODE_START_CHARACTER, self.ERROR_CODE_END_CHARACTER)
            return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.NOT_FOUND_STRING, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time), error=True, error_code=int(error_code))

        formatted_get_response = "Element '{0}' was retrieved succesfully.".format(element_id)

        # Succesful Pandora Transaction
        return PandoraResponse(http_status_code=request_response.status_code, response=formatted_get_response, result=response, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time))

    def remove_element(self, element_container_name: str, element_id: str) -> PandoraResponse:

        request_endpoint = '{0}://{1}:{2}/{3}/{4}/{5}/{6}'.format(self.HTTP, self.server_ip_address, self.server_port_number, self.PANDORA, self.DELETE_ELEMENT, element_container_name, element_id)
        request_response = requests.delete(request_endpoint)
        
        # Throw fast exception
        if(request_response.status_code != self.OK_CODE and self.throw_exceptions): 
            raise Exception(request_response.text)

        # Not Valid Request or Unknown Error
        if(request_response.status_code != self.OK_CODE and request_response.status_code != self.INTERNAL_ERROR_CODE):
            return PandoraResponse(http_status_code=request_response.status_code, response=request_response.text, result=self.FAILED_RESULT, error=True, error_code=self.UNKNOWN_PANDORA_ERROR_CODE)

        # General Request Response properties
        response = request_response.text[request_response.text.index('|')+self.RESPONSE_SEPARATION_LENGTH:]
        transaction_code = get_between(request_response.text, self.TRANSACTION_CODE_START_CHARACTER, self.TRANSACTION_CODE_END_CHARACTER)
        server_ellapsed_time = get_between(request_response.text, self.SERVER_ELLAPSED_TIME_START_CHARACTER, self.SERVER_ELLAPSED_TIME_END_CHARACTER)

        # Pandora Internal Error
        if(request_response.status_code == self.INTERNAL_ERROR_CODE): 
            error_code = get_between(request_response.text, self.ERROR_CODE_START_CHARACTER, self.ERROR_CODE_END_CHARACTER)
            return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.FAILED_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time), error=True, error_code=int(error_code))

        # Succesful Pandora Transaction
        return PandoraResponse(http_status_code=request_response.status_code, response=response, result=self.SUCCESFUL_RESULT, transaction_code=transaction_code, server_ellapsed_time=float(server_ellapsed_time))
