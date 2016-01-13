import cv2
from pykinect2 import PyKinectV2
from pykinect2.PyKinectV2 import *
from pykinect2 import PyKinectRuntime
import numpy as np

# Initialize Kinect
kinect = PyKinectRuntime.PyKinectRuntime(PyKinectV2.FrameSourceTypes_Depth | PyKinectV2.FrameSourceTypes_Color)
print(type(kinect))
# Get one color picture frame
frame = None
while(True):
	if (kinect.has_new_depth_frame()):
		frame = kinect.get_last_depth_frame()
		print(frame.view())
		break