# Multiple plot function
#
# ggplot objects can be passed in ..., or to plotlist (as a list of ggplot objects)
# - cols:   Number of columns in layout
# - layout: A matrix specifying the layout. If present, 'cols' is ignored.
#
# If the layout is something like matrix(c(1,2,3,3), nrow=2, byrow=TRUE),
# then plot 1 will go in the upper left, 2 will go in the upper right, and
# 3 will go all the way across the bottom.
#
multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)
  
  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)
  
  numPlots = length(plots)
  
  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  
  if (numPlots==1) {
    print(plots[[1]])
    
  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    
    # Make each plot, in the correct location
    #pdf(file, width=10, height=5*numPlots)
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
    #dev.off()
  }
}

# Making plots
mkPlots <- function()
{
  d <- dataset
  
  # NOTE: Title should not be necessary for every single plot. Caption should do.
  
  mkSinglePlot <- function(var, xLabel) {
    
    return (ggplot(d, aes(x=d[[var]], group = c )) + geom_density(alpha=.5,aes(fill=c, colour = c))+labs(x=xLabel, y="Density") +  
            scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')) 
  }
  p1 <- ggplot(d, aes(x=V1, group = c )) + geom_density(alpha=.5,aes(fill=c, colour = c))+labs(x="Component 1", y="Density") +  
      scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')
  
  p2 <- ggplot(d, aes(x=V2, group = c )) + geom_density(alpha=.5, aes(fill=c, colour = c))+labs(x="Component 2", y="Density") +  
    scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')
  p3 <-ggplot(d, aes(x=V3, group = c )) + geom_density(alpha=.5, aes(fill=c, colour = c))+labs(x="Component 3", y="Density") +  
    scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')
    scale_colour_discrete(name='Class')
  p4 <- ggplot(d, aes(x=V4, group = c )) + geom_density(alpha=.5, aes(fill=c, colour = c))+labs(x="Component 4", y="Density") +  
    scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')
  p5 <- ggplot(d, aes(x=V5, group = c )) + geom_density(alpha=.5, aes(fill=c, colour = c))+labs(x="Component 5", y="Density") +  
    scale_fill_discrete(name='Class')+scale_color_discrete(guide='none')
  
  pdf("ML-plots.pdf", width=6, height=2*5) # Original: w=12 h=4*5
  multiplot(p1, p2, p3, p4, p5, cols=1)
  dev.off()
}

ggpairs_test <- function()
{
  library(GGally)
  custom_car <- ggpairs(mtcars[,c("mpg","wt","cyl")], upper = "blank", title = "Custom Example")
  # ggplot example taken from example(geom_text)
  
  plot <- ggplot2::ggplot(mtcars, ggplot2::aes(x=wt, y=mpg, label=rownames(mtcars)))
  plot <- plot +
    ggplot2::geom_text(ggplot2::aes(colour=factor(cyl)), size = 3) +
    ggplot2::scale_colour_discrete(l=40)
  custom_car <- putPlot(custom_car, plot, 1, 2)
  
  personal_plot <- ggplot (iris, aes (x = Sepal.Length, y = Sepal.Width, fill = Species)) + stat_binhex (bins=5, aes (alpha = ..count..)) 
  custom_car <- putPlot(custom_car, personal_plot, 1, 1)
  custom_car 
}

# Conditioned plot
#hexplom(~dataset1[,1:5] | c, data = dataset1, xbins = 25, colramp = magent)

mkResultsHist <- function()
{
  results <- read.delim("~/Proiects/ML_final/FinalProject/src/comparison/results/results.txt")
  # Sort by accuracy
  results <- transform(results, 
              ClassifierType = reorder(ClassifierType, Accuracy)) 
  
  # You should define a palette here
  
  pdf("ML-results.pdf", width=5, height=3) # Original: w=12 h=4*5
  
  p<- ggplot(results, aes(x=ClassifierType, y=Accuracy)) + 
      geom_bar(stat="identity", width = 0.15,  aes(fill = ClassifierType, colour=ClassifierType)) +
      coord_flip() + scale_fill_discrete(guide='none') + scale_colour_discrete(guide='none')
  
  print(p)
  dev.off()
}

mkTimesHist <- function()
{
  results <- read.delim("~/Proiects/ML_final/FinalProject/src/comparison/results/results.txt")
  # Remove SVM
  results <- results[-c(6),]
  # Sort by accuracy
  results$TotalTime = results$TrainingTime+results$TestingTime
  sortedTTime <- sort(results$TotalTime)
  results <- results[match(sortedTTime, results$TotalTime),]
  #results <- transform(results, 
   #                    ClassifierType = reorder(ClassifierType, TotalTime)) 
  
  # You should define a palette here
  
  pdf("ML-times.pdf", width=8, height=3.55) # Original: w=12 h=4*5
  
  p<- ggplot(data=results) + 
    geom_histogram(aes(x=ClassifierType, y=TrainingTime, fill="red"), stat="identity", width = 0.15) +
    geom_histogram(aes(x=ClassifierType, y=TestingTime, fill="blue"), stat="identity", width = 0.15)   +
    coord_flip() + scale_fill_manual(labels=c("Training Time", "Testing Time"),
                                       values=c("#FF4040", "#436EEE"), name="") +
    scale_colour_discrete(guide='none') + ylab("Time (s)")
  
  print(p)
  dev.off()
}




mkHexPlom <- function()
{
  pdf("ML-pairs.pdf", width=18, height=9) # Original: w=12 h=4*5
  #p<-hexplom(~dataset2[,1:5] , data = dataset2, panel = panel.superpose,
  #xbins = 10, groups = c, alpha=0.25)  # XXX: legend?
  p <- hexplom(~dataset[,1:5] | c, data = dataset, xbins = 20, colramp = magent)
  print(p)
  dev.off()
}


