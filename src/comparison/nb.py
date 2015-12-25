from classifierutils import MLClassifier, MLData, miniTest, globalTest
import itertools

def quantizeData(nBins, data):
    newData = MLData()
    
    # For each row
    for x, y in itertools.izip(data.X, data.Y):
        # For each component, associate a discrete number
        newx = [ int(nBins*(x_component-.000001)) for x_component in x]
        newData.addRow(newx, y)

    #print '\n'.join([str(x)+ ", " + str(origx) for (x, origx) in zip(newData.X[:20], data.X[:20])])
    #print newData.Y[:20]

    return newData
        
    
    
    
    

class NBClassifier(MLClassifier):
    def train(self, params, trainingData):
        # Quantize data so that everything is discrete
        self.nBins = 1000  # Number of Bins
        trainingData = quantizeData(self.nBins, trainingData)

        N = trainingData.size()

        # Build class prior probs
        # priors is a dictionary because classes are -1 and 1
        self.priors = {}
        numClass = {}
        numClass[1] = len([y for y in trainingData.Y if y == 1])
        numClass[-1] = N - numClass[1]
        self.priors[1]= float(numClass[1]) / float(N)
        self.priors[-1] = 1-self.priors[1]
        

        nComponents = len(trainingData.X[0])
        self.nComponents = nComponents
        # initialization
        self.lklhoods = []
        for i in range(nComponents):            
            # Build Likelihoods (x_i | c_j)
            possiblePairs = [(x,c) for x in range(self.nBins) for c in [-1,1] ]
        
            lklhoods = dict([(p,0) for p in possiblePairs])
            self.lklhoods.append(lklhoods)
        
        # Build likelihoods scanning the data
        for x, y in itertools.izip(trainingData.X, trainingData.Y):
            for i in range(nComponents):
                v = x[i]
                #print v
                self.lklhoods[i][(v,y)] += 1

        # Normalize
        for i in range(nComponents):
            for k,v in self.lklhoods[i].items():
                _, y = k
                self.lklhoods[i][k] = float(v) / numClass[y]
                
    
        # Add smoothing parameter?
        
        #self.clf = self.clf.fit(trainingData.X, trainingData.Y)
    def predict(self, testData):
        #return self.clf.predict(testData.X)
        predictions = []
        quantizedData = quantizeData(self.nBins, testData)
        for x in quantizedData.X:
            estimates = { -1: 0, 1 : 0}
            for c in [-1,1]:
                likelihoods = [self.lklhoods[i][(x[i],c)] for i in range(self.nComponents)]
                likelihood = reduce(lambda a,b: a*b, likelihoods, 1.0)
                estimates[c] = self.priors[c]*likelihood
            if estimates[-1] > estimates[1]:
                predictions.append(-1)
            else:
                predictions.append(1)
        return predictions

if __name__ == '__main__':
    #miniTest("mini-test-nb", NBClassifier)
    globalTest("nb-time-accuracy", NBClassifier)
