from classifierutils import MLClassifier, MLData
from sklearn import tree

class DecTreeClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = tree.DecisionTreeClassifier()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)


if __name__ == '__main__':
    trainData = MLData('miniTrainData')
    testData =  MLData('miniTestData')
    print
    t = DecTreeClassifier()
    t.train({}, trainData)
    print t.getPerformance(testData)
