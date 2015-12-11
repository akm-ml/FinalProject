from classifierutils import MLClassifier, MLData, miniTest, globalTest
from sklearn import tree


class DecTreeClassifier(MLClassifier):
    def train(self, params, trainingData):
        self.clf = tree.DecisionTreeClassifier()
        self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        return self.clf.predict(testData.X)


if __name__ == '__main__':

    miniTest("mini-test-dec-tree", DecTreeClassifier)
    #globalTest("decision-tree-time-accuracy", DecTreeClassifier)
