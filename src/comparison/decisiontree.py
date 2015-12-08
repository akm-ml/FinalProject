from classifierutils import MLClassifier, MLData
from sklearn import tree

class DecTreeClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = tree.DecisionTreeClassifier()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)

def trainOnAllData():
    trainData = MLData('pr_data12')
    testData =  MLData('pr_data3')
    
    t = DecTreeClassifier()
    t.train({}, trainData)

    # Log result
    f = open('accuracy-dec-tree', 'w')
    f.write(str(t.getPerformance(testData)) + '\n')
    f.close()

def miniTest():
    trainData = MLData('miniTrainData')
    testData =  MLData('miniTestData')
    print
    t = DecTreeClassifier()
    t.train({}, trainData)
    print t.getPerformance(testData)


if __name__ == '__main__':
    trainOnAllData()
