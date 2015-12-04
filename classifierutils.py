# General infrastructure for ML algorithms


# Abstract class for classifiers
class MLClassifier:
    # For each MLClassifier (in the subclasses) we define the (static) range of 
    # possible parameters 
    def __init__(self):
        pass
    
    def getPerformance(self, testData):
        pass
    
    def train(self, params, trainingData):
        """ Given a dictionary of params it trains the ML accordingly"""
    
# Test all hyperparameters in range and returns best
class Learner:
    def __init__(self, classifierClass, trainData, devData, testData, logFile):
        self.classifierCLass = classifierClass
        self.trainData = trainData
        self.devData = devData
        self.testData = testData
        self.bestParams = None
        self.logFile = logFile
        
    def allPossibleParams(self):
        pass
    def findBestHyperParams(self):
        bestParams = None
        bestPerformance = -1
        for params in self.allPossibleParams():
            # Log stuff about the new simulator here
            # TODO
            
            sim = self.classifierClass()
            # XXX: Log time for training?
            sim.train(params)
            
            performance = sim.getPerformance(devData)
            if performance > bestPerformance:
                bestPerformace = performance
                bestParams = params
        
        # save best params
        self.bestParams = bestParams
        # Log about the best params/performance
        # TODO
    def go(self, testData):
        # Do the training with the test data (maybe we can save it from before?)
        # and test it on the test data
        
        # return performance
        
        
        
if __name__ == '__main__':
    print("Hello, this is a ML program!")
