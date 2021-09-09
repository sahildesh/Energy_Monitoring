import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn import metrics


df = pd.read_csv("C:/Users/Manga/Desktop/project/dumy.csv")

X_train=df['Day']
Y_train=df['Current']

X_train=X_train.values.reshape(-1,1)
#X_train,X_test,Y_train,Y_test=train_test_split(X_test,Y_test,test_size=0.3,random_state=101)

lm= LinearRegression()
lm.fit(X_train,Y_train)
#lm.fit(X)
#print(lm.intercept_)
#print(lm.coef_)
print(X_train,Y_train)
print(lm.predict(7))

#plt.scatter(Y_test,prediction)
