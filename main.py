import cv2
from pykinect2 import PyKinectV2
from pykinect2.PyKinectV2 import *
from pykinect2 import PyKinectRuntime
import numpy as np

kinect = PyKinectRuntime.PyKinectRuntime(PyKinectV2.FrameSourceTypes_Color | PyKinectV2.FrameSourceTypes_Body)

while(True):
	if(kinect.has_new_color_frame()):
		frame = kinect.get_last_color_frame()
		print(frame.view())
#img=frame.view(np.uint8).reshape(frame.shape+(4,))[..., :3]