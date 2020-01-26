import numpy as np
from matplotlib import pyplot as plt

def histogramme(histogramme):

    histo = np.loadtxt(histogramme)
    plt.hist(histo , bins = [1,2,3,4,5,8,9,10], facecolor = 'blue')
    plt.title('Qualité des éléments du maillage')
    plt.savefig("histogramme_qualité.png")
    plt.show()

    return 0
