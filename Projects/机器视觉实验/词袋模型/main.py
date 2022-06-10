import os
import cv2 as cv
import numpy as np
import glob
from tqdm import tqdm
from sklearn.svm import SVC
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.cluster import MiniBatchKMeans
import math

path = "dataset/"
num_of_bag = 200


def load_data(path):
    """
    载入数据
    :param path: 数据库路径
    :return: 训练路径，训练标签，测试路径，测试标签
    """
    categories = os.listdir(path)
    train_label = []
    test_label = []
    train_paths = []
    test_paths = []

    for category in categories:
        img_paths = glob.glob(path + category + '/*.jpg')
        train_paths.extend(img_paths[:150])
        test_paths.extend(img_paths[150:])

        train_label.extend([category] * 150)
        test_label.extend([category] * (len(img_paths) - 150))

    return train_paths, train_label, test_paths, test_label


def normalize_desc(desc):
    """
    将特征描述符归一化
    :param desc: 特征描述符
    :return: 归一化后的描述符
    """
    for i in range(len(desc)):
        norm = np.linalg.norm(desc[i])
        if norm > 1:
            desc[i] /= float(norm)
    return desc


def SIFT(paths):
    """
    提取输入路径的图片并归一化
    :param paths: 输入路径
    :return: 归一化的SIFT特征描述符，SIFT特征点，图片的形状
    """
    sift = cv.SIFT_create()
    descriptors = []
    keypoints = []
    shapes = []

    for img_path in tqdm(paths):
        img = cv.imread(img_path)
        shapes.append(img.shape[:2])

        keypoint, descriptor = sift.detectAndCompute(img, None)
        descriptors.append(descriptor)
        keypoints.append(keypoint)
        descriptors = normalize_desc(descriptors)
    return descriptors, keypoints, shapes


def get_BoW(train_desc):
    """
    通过聚类算法，生成词袋模型
    :param train_desc: 训练集的特征描述符
    :return: 聚类模型
    """
    data = []
    for i in range(len(train_desc)):
        for j in range(len(train_desc[i])):
            data.append(train_desc[i][j])
    data = np.array(data)
    kmeans = MiniBatchKMeans(n_clusters=num_of_bag, random_state=42)
    kmeans.fit(data)
    return kmeans


def get_SPM_feature(desc, kp, shape):
    """
    生成每张图片的SPM特征向量
    :param desc: SIFT描述符列表
    :param kp: SIFT特征点列表
    :param shape: 图像形状列表
    :return: SPM特征向量
    """
    hists = np.array([]).reshape(0, num_of_bag * 21)

    for i in tqdm(range(len(desc))):
        hist = SPM(desc[i], kp[i], shape[i])
        hist = hist.reshape(1, 21 * num_of_bag)
        hists = np.append(hists, hist, axis=0)

    return hists


def SPM(descs, kps, shape):
    """
    使用SPM算法，统计不同尺度下的直方图
    :param descs:图片的SIFT特征向量
    :param kps: 图片的SIFT特征点
    :param shape: 图片的形状
    :return: SPM算法生成的特征向量
    """
    center = kmeans.cluster_centers_

    # 划分3个尺度
    level2 = np.zeros((16, num_of_bag))
    level1 = np.zeros((4, num_of_bag))
    level0 = np.zeros((1, num_of_bag))

    # 图片分格
    step_x = math.ceil(shape[1] / 4)
    step_y = math.ceil(shape[0] / 4)

    for i in range(len(descs)):
        desc = descs[i]
        kp = kps[i]
        x, y = kp.pt  # 特征点的坐标

        diff = np.tile(desc, (num_of_bag, 1)) - center
        distance = np.sum(np.square(diff), axis=1)
        index = np.argmin(distance)  # 找到离这个特征点距离最近的词汇中心（即划分此特征点的类别）

        cell = math.floor(x / step_x) + math.floor(y / step_y) * 4  # 特征点所在区域
        level2[cell][index] += 1  # 投票

    for i, idx in enumerate([0, 2, 8, 10]):
        level1[i] = level2[idx] + level2[idx + 1] + level2[idx + 4] + level2[idx + 5]

    for i in range(4):
        level0[0] += level1[i]

    result = np.array([]).reshape(0, num_of_bag)
    # 加权求和
    result = np.append(result, level0 * 0.25, axis=0)
    result = np.append(result, level1 * 0.25, axis=0)
    result = np.append(result, level2 * 0.5, axis=0)
    return result


def evaluate(model):
    """
    评估实验结果
    :param model: 训练得到的模型
    """
    pred_labels = model.predict(test_bin)

    report = classification_report(test_label, pred_labels)
    conf_mx = confusion_matrix(test_label, pred_labels)
    print(report)
    print(conf_mx)


if __name__ == "__main__":
    # 提取数据集中的样本，并划分训练集和测试集
    train_paths, train_label, test_paths, test_label = load_data(path)

    # 提取训练集图片的 SIFT 特征点，并将 SIFT 特征点向量归一化
    print("Extract SIFT features...")
    train_desc, train_keypoint, train_shape = SIFT(train_paths)
    test_desc, test_keypoint, test_shape = SIFT(test_paths)

    # 将所有SIFT特征点使用聚类算法分为n类
    # 并将n类特征点的中心点作为视觉词汇，生成词袋
    kmeans = get_BoW(train_desc)

    # 使用 SPM 算法生成图片的特征向量
    print("Executing SPM algorithm...")
    train_bin = get_SPM_feature(train_desc, train_keypoint, train_shape)
    test_bin = get_SPM_feature(test_desc, test_keypoint, test_shape)

    # 将图片的特征向量作为数据集，使用支持向量机算法完成分类任务
    svm = SVC(kernel='rbf', C=10, random_state=42)
    svm.fit(train_bin, train_label)

    # 评估结果
    evaluate(svm)
