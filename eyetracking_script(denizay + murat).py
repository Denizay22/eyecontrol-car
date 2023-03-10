import cv2 as cv
import mediapipe as mp
import numpy as np
import math

#RIGHT_EYE = [362,382,381,380,374,373,390,249,263,466,388,387,386,385,384,398]
LEFT_EYE = [33,7,163,144,145,153,154,155,133,173,157,158,159,160,161,246]
#RIGHT_IRIS = [474, 475, 476, 477]
LEFT_IRIS = [469, 470, 471, 472]
LEFT_MLEFT = [33]
LEFT_MRIGHT = [133]
LEFT_MUP = [159]
LEFT_MDOWN = [145]
mp_face_mesh = mp.solutions.face_mesh

def euc_dist(p1, p2):
    x1, y1 = p1.ravel()
    x2, y2 = p2.ravel()
    distance = math.sqrt((x2-x1)**2 + (y2-y1)**2)
    return distance

def iris_position(iris_center, eye_mright, eye_mleft, eye_mup, eye_mdown):
    ##POSITION DEFINATIONS
    ##eğer horizontal center ise: 
    ##  eğer vertical center ise: center
    ##
    center_to_right_dist = round(euc_dist(iris_center, eye_mright),2)
    center_to_up_dist = round(euc_dist(iris_center, eye_mup),2)
    up_to_down_dist = euc_dist(eye_mup, eye_mdown)
    left_to_right_dist = euc_dist(eye_mleft, eye_mright)
    vertical_ratio = center_to_up_dist / up_to_down_dist
    horizontal_ratio = center_to_right_dist / left_to_right_dist
    
    iris_position = ""
    if left_to_right_dist / up_to_down_dist > 4:
        iris_position = "down"
    elif horizontal_ratio < 0.39:
        iris_position = "right"
    elif horizontal_ratio > 0.61:
        iris_position = "left"
    else:
        if vertical_ratio < 0.39:
            iris_position = "up"
        else:
            iris_position = "center"
    print(
                "\nHorizontal ratio: " + str(horizontal_ratio) + 
                "\nVertical ratio: " + str(vertical_ratio) + 
                "\nEye distance ratio: " + str(left_to_right_dist/up_to_down_dist) + 
                "\nEstimated iris pos: " + iris_position
            )
    print(
        "\nUp coord: " + str(eye_mup) + 
        "\nDown coord: " + str(eye_mdown) + 
        "\nLeft coord: " + str(eye_mleft) + 
        "\nRight coord: " + str(eye_mright)
    )
    ##TODO test, düşük ışıkta çok iyi çalışmıyor
    return iris_position


def frame_process_2(frame):
    img_h, img_w = frame.shape[:2]
    with mp_face_mesh.FaceMesh(
        static_image_mode=True,
        max_num_faces=1,
        refine_landmarks=True,
        min_detection_confidence=0.5,
        min_tracking_confidence=0.9
        ##TODO en iyi confidence değerini önceden çekilmiş fotoğraflar ile test et!!
    ) as face_mesh:
        results = face_mesh.process(cv.cvtColor(frame, cv.COLOR_BGR2RGB))
        if results.multi_face_landmarks:
            mesh_points = np.array([np.multiply([p.x, p.y], [img_w, img_h]).astype(int) for p in results.multi_face_landmarks[0].landmark])
            (left_cx, left_cy), left_radius = cv.minEnclosingCircle(mesh_points[LEFT_IRIS])
            left_center = np.array([left_cx, left_cy], dtype=np.int32)
            cv.circle(frame, left_center, int(left_radius), (255,0,0), 1, cv.LINE_AA)
            cv.circle(frame, left_center, 3, (0,255,0), -1, cv.LINE_AA)
            
            cv.circle(frame, mesh_points[LEFT_MLEFT][0], 3, (255,255,255), -1, cv.LINE_AA)
            cv.circle(frame, mesh_points[LEFT_MRIGHT][0], 3, (255,255,255), -1, cv.LINE_AA)
            cv.circle(frame, mesh_points[LEFT_MDOWN][0], 3, (255,255,255), -1, cv.LINE_AA)
            cv.circle(frame, mesh_points[LEFT_MUP][0], 3, (255,255,255), -1, cv.LINE_AA)
            iris_position(left_center, mesh_points[LEFT_MRIGHT][0], mesh_points[LEFT_MLEFT][0], mesh_points[LEFT_MUP][0], mesh_points[LEFT_MDOWN][0])
            
            cv.imshow('shot', frame)
            

    
def video_capture_2():
    capture = cv.VideoCapture(0)
    fps = 30
    delay = int(1000/fps)
    while True:
        ret, frame = capture.read()
        frame = cv.flip(frame, 1)
        cv.imshow('frame', frame)
        key = cv.waitKey(delay)
        if key == ord('q') or key == ord('Q'):
            break
        elif key == ord('y') or key == ord('Y'):
            frame_process_2(frame)
    capture.release()
    cv.destroyAllWindows()
    
video_capture_2()