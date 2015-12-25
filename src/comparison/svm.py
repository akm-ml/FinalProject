from classifierutils import MLClassifier, MLData, miniTest, globalTest, globalTest_bis
import numpy as np
from sklearn import svm

class SVMClassifier(MLClassifier):
    def train(self, params, trainData):
        self.clf = svm.SVC()
        self.clf  = self.clf.fit(trainData.X, trainData.Y)

    def predict(self, testData):
        return self.clf.predict(testData.X)

if __name__ == '__main__':
    #miniTest("mini-test-svm", SVMClassifier)
    globalTest_bis("svm-time-accuracy", SVMClassifier)
