#插入库
#import sklearn.datasets
from sklearn.datasets import load_iris  # 导入加载iris数据集的函数
'''
不能直接插入sklearn.datasets,因为sklearn.datasets是一个大文件, load_iris 不在 sklearn 根目录下，而是在 sklearn.datasets 子模块中，
直接导入并不能使用 load_iris 函数
若用sklearn.datasets,则在加载数据的时候应该这样写:iris = sklearn.datasets.load_iris()
'''
from sklearn.svm import SVC  # 导入SVM分类器,用来对数据进行分类
from sklearn.model_selection import train_test_split
'''
插入这个库用于对数据集进行训练集和测试集的划分,默认是将数据集按80%20%的比例划分。
注意:这里的对数据分类并不只是分类,而是对数据进行训练和测试,训练集和测试集的划分是为了评估模型的性能。
'''

iris = load_iris() # 加载数据集，注意，这里的iris是一个字典，包含了所有的信息

x = iris.data #x是从iris字典中提取的特征数据,每朵花有4个数值特征（花萼长、花萼宽、花瓣长、花瓣宽）

y = iris.target #y是从iris字典中提取的目标标签,每个样本的类别标签（0是山鸢尾，1是杂色鸢尾，2是维吉尼亚鸢尾）

'''
打印查看数据集信息来验证数据集是否加载成功
print(f"特征维度: {x.shape}")
print(f"类别: {target_names}")
'''

x_train, x_test, y_train, y_test = train_test_split(x, y, random_state=42)
#分别对x和y进行训练集和测试集的划分
#train_test_split :打乱样本顺序，在按照75%作为训练集，25%作为测试集的比例对样本进行划分。
#random_state=42 表示设置随机种子，注意:设置随机种子是为了确保每次运行代码时，随机数的生成结果都是相同的,从而方便复现，方便调试。而不是用来测试模型是否准确的
model = SVC(kernel='linear')  # 创建 SVM 模型，其中SVC是SVM分类器, kernel='linear'表示使用线性核函数
model.fit(x_train, y_train)   # 训练，即让model适应训练集中的数据

y_predict = model.predict(x_test)#预测测试集,由x_test得到预测的y_predict

from sklearn.metrics import accuracy_score#插入能计算准确率的函数
acc = accuracy_score(y_test, y_predict)# 用预测的y_predict和真实的y_test比较，来计算准确率
print("准确率:", acc)

predicted_species = iris.target_names[y_predict]#这里的y_predict是一个数组，每个元素是预测的类别
print("预测的类别:")
for i ,species in enumerate(predicted_species):#通过遍历，打印出分类结果
    print(f"样本{i}: {species}")
#enumerate()函数可以同时获取索引和元素，这里的索引（位置）就是样本的序号，元素就是预测的类别
