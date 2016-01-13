import cv2
import numpy as np

background = cv2.imread('Background_Color.jpg')
frame = cv2.imread('StableOutput_Color1.jpg')

diff = cv2.absdiff(frame, background)
cv2.imwrite("diff.jpg", diff)

#Hough circle detection using greyscale img
min_radius = 50
max_radius = 1000

# gray_frame = cv2.cvtColor(diff, cv2.COLOR_BGR2GRAY)

# cv2.imwrite("OutputGray.jpg", gray_frame)

lower_blue = np.array([20, 20, 20])
upper_blue = np.array([230, 230, 230])

gray_frame = cv2.inRange(diff, lower_blue, upper_blue)
gray_frame = cv2.medianBlur(gray_frame, 11)

cv2.imwrite("gray.jpg", gray_frame)

balls = cv2.HoughCircles(gray_frame, cv2.cv.CV_HOUGH_GRADIENT, 5, minDist=25, param1=100, param2=100, minRadius=5, maxRadius=30)

detector = cv2.SimpleBlobDetector()

keypoints = detector.detect(gray_frame)

print(keypoints)

im_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

cv2.imwrite("blobballs.jpg", im_with_keypoints)

print("Detected balls:")
print(balls)
balls = np.round(balls[0, :]).astype("int")

#Display detected balls
for (x, y, z) in balls:
	cv2.circle(diff, (x, y), z, (0, 255, 0), 4)

cv2.imwrite("DetectedBalls.jpg", diff)
