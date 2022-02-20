import cv2
import time

def snapShotCt(camera_idx = 0): # camera_idx的作用是选择摄像头。如果为0则使用内置摄像头&#xff0c;比如笔记本的摄像头&#xff0c;用1或其他的就是切换摄像头。
    cap = cv2.VideoCapture(camera_idx)
    ret, frame = cap.read() # cao.read()返回两个值&#xff0c;第一个存储一个bool值&#xff0c;表示拍摄成功与否。第二个是当前截取的图片帧。
    cv2.imwrite("result.jpg", frame) # 写入图片
    cap.release()

snapShotCt(0)