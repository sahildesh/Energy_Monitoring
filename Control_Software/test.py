import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn import metrics

df = pd.read_csv("C:/Users/Manga/Desktop/test.csv")
df1 = pd.read_csv("C:/Users/Manga/Desktop/test1.csv")
df2 = pd.read_csv("C:/Users/Manga/Desktop/Project/dumy_light.csv")

X1=df2['Sr.No']
Y1=df2['Current ']
print(X1)
X1=X1.values.reshape(-1,1)

#print(df.columns)
X_train=df['col1']
Y_train=df['col2']
X_train=X_train.values.reshape(-1,1)

X_test=df1['col1']
X_test=X_test.values.reshape(-1,1)
Y_test=df1['col2']

#print(X)
#X_train,X_test,Y_train,Y_test=train_test_split(X,Y,test_size=0.3,random_state=101)
#print(X_test)
#print(Y_test)
#Y_train.values.reshape(-1,1)
lm= LinearRegression()
#lm.fit(X_train,Y_train)

lm.fit(X1,Y1)


print(lm.intercept_)
print(lm.coef_)
#prediction=lm.predict(X_test)

#plt.scatter(Y_test,prediction)
#plt.show()

#print(prediction)


#print(metrics.mean_absolute_error(prediction,Y_test))
#print(metrics.mean_squared_error(Y_test,prediction))
#print(np.sqrt(metrics.mean_squared_error(Y_test,prediction)))