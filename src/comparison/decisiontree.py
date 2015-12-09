from classifierutils import MLClassifier, MLData
from sklearn import tree
import time

class DecTreeClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = tree.DecisionTreeClassifier()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)


def logOnFile(fn, accuracy, trainTime, testTime):
    # Log result
    f = open(fn, 'w')
    f.write("Accuracy: " + str(accuracy) + '\n')
    f.write("Training time: " + str(trainTime) + '\n')
    f.write("Testing time: " + str(testTime) + '\n')
    f.close()


def trainOnAllData():
    trainData = MLData('pr_data12')
    testData =  MLData('pr_data3')

    startTime = time.time()
    t = DecTreeClassifier()
    t.train({}, trainData)
    trainTimeOffset = time.time()
    trainTime = trainTimeOffset - startTime
    
    perf =  t.getPerformance(testData)
    testTime = time.time() - trainTimeOffset

    logOnFile("decision-tree-time-accuracy", perf, trainTime, testTime)

def miniTest():
    trainData = MLData('miniTrainData')
    testData =  MLData('miniTestData')
    

    startTime = time.time()
    t = DecTreeClassifier()
    t.train({}, trainData)
    trainTimeOffset = time.time()
    trainTime = trainTimeOffset - startTime
    
    perf =  t.getPerformance(testData)
    testTime = time.time() - trainTimeOffset

    logOnFile("mini-test-dec-tree", perf, trainTime, testTime)



if __name__ == '__main__':
    trainOnAllData()
    #miniTest()
