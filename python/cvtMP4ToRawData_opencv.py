'''
Author: zhuqingquan
Date: 2024-10-24 
Description: 使用opencv将视频文件转换为RGB24格式的原始数据。稍作修改也可以从摄像头获取数据保存成RGB24格式的原始数据。
'''
import cv2
import numpy as np

def cvt(src_file, max_frames=-1):
    # Open the video capture
    cap = cv2.VideoCapture(src_file)  # Replace 'video.mp4' with your video file or use 0 for webcam

    if not cap.isOpened():
        print("Error: Could not open video.")
        exit()

    frame_count = 0
    f_out = None
    while True:
        # Read a frame
        ret, frame = cap.read()
        if not ret:
            print("Error: Could not read frame.")
            cap.release()
            exit()

        if f_out is None:
            # Save the frame as an RGB24 file
            height, width, _ = frame.shape
            rgb24_filename = '{0}_{1}x{2}.rgb'.format(src_file, width, height)
            f_out = open(rgb24_filename, 'wb')
        frame_count += 1
        # Convert the frame from BGR to RGB
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        f_out.write(rgb_frame.tobytes())
        if max_frames > 0 and frame_count >= max_frames:
            break

    print('Frame saved as {0}'.format(rgb24_filename))
    # Release the video capture
    cap.release()

if __name__=='__main__':
    cvt('D:\\data_for_test\\66affdda71c32397bce45e963276df3f.mp4', 100)