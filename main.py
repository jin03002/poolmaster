import cv2
from pykinect2 import PyKinectV2
from pykinect2.PyKinectV2 import *
from pykinect2 import PyKinectRuntime
import numpy as np

def main():

	#Initialize Kinect
	kinect = PyKinectRuntime.PyKinectRuntime(PyKinectV2.FrameSourceTypes_Color | PyKinectV2.FrameSourceTypes_Body)

	#Get one color picture frame
	frame=None
	while(True):
		if(kinect.has_new_color_frame()):
			frame = kinect.get_last_color_frame()
			print(frame.view())
			break

	#Convert to opencv
	frame = frame.reshape((1080, 1920, 4))

	#Create opencv output
	output = frame.copy()

	#Output to file for testing
	cv2.imwrite("Output.jpg", frame)

	#Hough circle detection using greyscale img
	min_radius = 50
	max_radius = 1000

	gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	cv2.imwrite("OutputGray.jpg", gray_frame)

	balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 1, 20, minRadius=0, maxRadius=1000)

	print("Detected balls")
	print(balls)
	balls = np.round(balls[0, :]).astype("int")

	#Display detected balls
	for (x, y, z) in balls:
		cv2.circle(frame, (x, y), z, (0, 255, 0), 2)

	cv2.imshow("output", frame)
	cv2.waitKey(0)

if __name__=="__main__": main()