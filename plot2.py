#!/usr/bin/python2.7

from __future__ import division
import scipy.io as spio
import scipy.sparse as spa
import itertools
import numpy as np
import sys
from math import ceil, sqrt
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable

def binning(M, blockSize):
		size=M.shape[0]
		binNbr=int(ceil(size/blockSize))
		matSize=M.shape[0]
		blockSize=ceil(matSize/binNbr)
		bins=np.zeros((binNbr, binNbr), dtype=int)
		for i,j in itertools.izip(M.row, M.col):
				bins[(i//blockSize,j//blockSize)]+=1
		return bins

def nnzBlocks(binning):
		count=0
		for val in np.nditer(binning):
				if val!=0:
						count+=1
		return count

def draw(bins, blockSize):

		count=nnzBlocks(bins)
		blockNbr=len(bins)

		# CUSTOM COlORMAP BASED ON JET WITH WHITE AT THE BOTTOM
		cdict = {'red': ((0., 1, 1),
								 (1e-60, 0, 0),
								 (0.11, 0, 0),
								 (0.66, 1, 1),
								 (0.89, 1, 1),
								 (1, 0.5, 0.5)),
				 'green': ((0., 1, 1),
									 (1e-60, 0, 0),
									 (0.11, 0, 0),
									 (0.375, 1, 1),
									 (0.64, 1, 1),
									 (0.91, 0, 0),
									 (1, 0, 0)),
				 'blue': ((0., 1, 1),
									(1e-60, 1, 1),
									(0.11, 1, 1),
									(0.34, 1, 1),
									(0.65, 0, 0),
									(1, 0, 0))}

		my_cmap = matplotlib.colors.LinearSegmentedColormap('my_colormap',cdict,256)

		fig=plt.figure()
		ax=plt.subplot(111)
		im=plt.imshow(bins, cmap=my_cmap, interpolation='none', vmin=0.0)

		plt.title("{2} - {0:,}x{0:,} blocks of size {3:,}x{3:,} ({1:,} non-zero blocks)".format(blockNbr, count, fname, blockSize))

		# COLORBAR OF THE RIGHT SIZE
		divider = make_axes_locatable(ax)
		cax = divider.append_axes("right", size="5%", pad=.5)
		plt.colorbar(im, cax=cax)
		plt.show()  

if __name__ == "__main__":

		fname=sys.argv[1]
		blockSize=int(sys.argv[2])

		M = spio.mmread(fname)
		print("Loaded: {0} (size {1:,}, {2:,} non-zeros)".format(fname, M.shape[0], M.nnz))

		bins=binning(M, blockSize)
		print("Binning computed: {0[0]:,}x{0[0]:,} blocks of size {1}x{1} (total: {0[1]:,} blocks)".format((len(bins), len(bins)**2), blockSize))

		draw(bins, blockSize)