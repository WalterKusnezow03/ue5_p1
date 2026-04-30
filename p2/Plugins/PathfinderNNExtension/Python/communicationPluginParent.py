import os
import sys


#### find NNCommunicationPlugin working dir to launch tcp listener ####
BASE_DIR = os.path.dirname(__file__)

COMMUNICATION_PYTHON = os.path.abspath(
    os.path.join(BASE_DIR, "../../NNCommunicationPlugin/Python")
)

sys.path.insert(0, COMMUNICATION_PYTHON)

print("COMM PATH:", COMMUNICATION_PYTHON, "\n")

##### works as expected, ignore yellow line!
import nn_server
from NNBase import CNNBase

print("IMPORT OK \n")