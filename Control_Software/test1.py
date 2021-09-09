import pandas as pd     #data processing and data analysis tools
import numpy as np      #adding support for large, multi-dimensional arrays and matrices,
                        #collection of high-level mathematical functions
import csv
from sklearn.linear_model import LinearRegression

curr_threshold=4.5  #fixed
df = pd.read_csv("C:/Users/Manga/Desktop/Project/test_input.csv") #read the csv file already present and store in data frame.

X=df['Day'] #store day in X
Y=df['Current'] #store current in Y

#if the deviation is more than expected.
for i in range(len(Y)-2):
    if ((Y[i] - Y[i + 1]) > 0.2):
        print("Error" , i)

#convert object into 1 dimesional array object
X=X.values.reshape(-1,1)

#fit data in linear regression model.
lm=LinearRegression()
lm.fit(X,Y)

#create 2D array to store predicted data.
myData=[["Day","Current"]]
Day=X[-1][-1]+1 #start day from last day in train data set.

#run loop from 'Day' day to 200th day and store the predicted current value of each day
for x in range(Day,200):
    a=lm.predict(x)[-1]
    myData.append([x,a])

myData=np.array(myData) #convert myData object to numpy array
myFile = open('predicted.csv', 'w') #open a new CSV file to store predicted values with write permission.

#write data to file.
with myFile:
    writer = csv.writer(myFile)
    writer.writerows(myData)

#---------------------process the data of the predicted file.

df1 = pd.read_csv("predicted.csv") #store data from predicted file in 'df1' dataframe.
current = df1["Current"] #store current attribute.
day=df1["Day"] #store day attribute.

#check whether current is less than threshold and give appropriate error.
for i in range(len(current)):
    if((current[i]-current[i+1])>0.2):
        print("Error")
    if(current[i]>curr_threshold):
        #if(i==1):
        print("Device will need maintainence after", day[i],"days")
        #print("Device will need maintainence after",day[i],"days")
        break