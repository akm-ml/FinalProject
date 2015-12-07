from classifierutils import MLClassifier, MLData
import numpy as np
from sklearn.neighbors import KNeighborsClassifier

class NNClassifier(MLClassifier):
    def train(self, params, trainData):
        # XXX: set n_neighbors
        self.neigh = KNeighborsClassifier(n_neighbors = 1)
        self.neigh  = self.neigh.fit(trainData.X, trainData.Y)

    def predict(self, testData):
        return self.neigh.predict(testData.X)

if __name__ == '__main__':
    trainData = MLData('miniTrainData')
    testData =  MLData('miniTestData')
    
    nn = NNClassifier()
    nn.train({}, trainData)
    print nn.getPerformance(testData)
