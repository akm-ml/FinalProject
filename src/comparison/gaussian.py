from classifierutils import MLClassifier, MLData, miniTest, globalTest
from sklearn.naive_bayes import GaussianNB

class GaussianClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = GaussianNB()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)

if __name__ == '__main__':
    #miniTest("mini-test-nb", GaussianClassifier)
    globalTest("nb-time-accuracy", GaussianClassifier)
