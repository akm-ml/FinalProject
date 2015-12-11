from classifierutils import MLClassifier, MLData, miniTest, globalTest
from sklearn.linear_model import LogisticRegression

class LogRegClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = LogisticRegression()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)

if __name__ == '__main__':
    #miniTest("mini-test-logreg", LogRegClassifier)
    globalTest("logreg-time-accuracy", LogRegClassifier)
