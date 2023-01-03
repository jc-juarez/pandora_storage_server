# *************************************
# Pandora Storage Server Python Client
# Client
# 'setup.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

from setuptools import setup, find_packages

VERSION = '0.0.1'
DESCRIPTION = 'Pandora Storage Server Client.'

# Package setup
setup(
    name="Pandora Storage Server Client",
    version=VERSION,
    author="Juan Carlos Juárez",
    author_email="<jc.juarezgarcia@outlook.com>",
    description=DESCRIPTION,
    long_description_content_type="text/markdown",
    long_description=LONG_DESCRIPTION,
    packages=find_packages(),
    install_requires=[],
    keywords=['Storage', 'Key-Value', 'Server', 'NoSQL', 'Schemaless', 'GNU/Linux'],
)