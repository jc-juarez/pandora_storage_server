# *************************************
# Pandora Storage Server Python Client
# Client
# 'setup.py'
# Author: Juan Carlos Juárez
# Contact: jc.juarezgarcia@outlook.com
# *************************************

from setuptools import setup
from pathlib import Path

VERSION = '0.1.2'
DESCRIPTION = 'Pandora Storage Server Client.'

# Read Content of README file
this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text()

# Package setup
setup(
    name="pandora-storage-server-client",
    version=VERSION,
    author="Juan Carlos Juárez",
    author_email="<jc.juarezgarcia@outlook.com>",
    description=DESCRIPTION,
    long_description_content_type="text/markdown",
    long_description=long_description,
    install_requires=["requests"],
    keywords=['Storage', 'Key-Value', 'Server', 'NoSQL', 'Schemaless', 'GNU/Linux'],
)