
import sys
sys.path.append('../')
from pycore.tikzeng import *

# defined your arch
arch = [
    to_head( '..' ),
    to_cor(),
    to_begin(),
    to_Conv("conv1", 32, 32, offset="(0,0,0)", to="(0,0,0)", height=32, depth=32, width=5 ,caption="Conv1"),
    to_Pool("pool1", offset="(0.5,0,0)", to="(conv1-east)" ,width=1, height=32, depth=32, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv1", "pool1"), 


    to_Conv("conv2", 32, 32, offset="(3,0,0)", to="(0,0,0)", height=32, depth=32, width=5 ,caption="Conv2"),
    to_Pool("pool2", offset="(0.5,0,0)", to="(conv2-east)" ,width=1, height=32, depth=32, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv2", "pool2"), 

    to_Conv("conv3", 16, 64, offset="(6,0,0)", to="(0,0,0)", height=16, depth=16, width=8 ,caption="Conv3"),
    to_Pool("pool3", offset="(0.4,0,0)", to="(conv3-east)" ,width=1, height=16, depth=16, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv3", "pool3"), 

    to_Conv("conv4",  16,64, offset="(9.5,0,0)", to="(0,0,0)", height=16, depth=16, width=8 ,caption="Conv4"),
    to_Pool("pool4", offset="(0.4,0,0)", to="(conv4-east)" ,width=1, height=16, depth=16, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv4", "pool4"), 


    to_Conv("conv5",  8, 128,offset="(13,0,0)", to="(0,0,0)", height=8, depth=8, width=11 ,caption="Conv5"),
    to_Pool("pool5", offset="(0.3,0,0)", to="(conv5-east)" ,width=1, height=8, depth=8, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv5", "pool5"), 


    to_Conv("conv6",  8, 128,offset="(16.5,0,0)", to="(0,0,0)", height=8, depth=8, width=11 ,caption="Conv6"),
    to_Pool("pool6", offset="(0.3,0,0)", to="(conv6-east)" ,width=1, height=8, depth=8, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv6", "pool6"), 

    to_Conv("conv7",  8, 256,offset="(20,0,0)", to="(0,0,0)", height=8, depth=8, width=15 ,caption="Conv7"),
    to_Pool("pool7", offset="(0.3,0,0)", to="(conv7-east)" ,width=1, height=8, depth=8, opacity=0.5, caption="BN+\nTanh"),
    to_connection( "conv7", "pool7"), 

    to_Conv("conv8",  8, 256,offset="(24.5,0,0)", to="(0,0,0)", height=8, depth=8, width=15 ,caption="Conv8"),
    to_Pool("pool8", offset="(0.3,0,0)", to="(conv8-east)" ,width=1, height=8, depth=8, opacity=0.5, caption="BN"),
    to_connection( "conv8", "pool8"), 


    to_Conv("conv9",  256,1, offset="(29.5,0,0)", to="(0,0,0)", height=1, depth=30, width=1 ,caption=""),
    # to_Pool("pool8", offset="(0.0,0,0)", to="(conv8-east)" ,width=0.3, height=1, depth=30, opacity=0.5, caption=""),
    # to_connection( "conv8", "pool8"), 


    to_connection( "pool1", "conv2"), 
    to_connection( "pool2", "conv3"), 
    to_connection( "pool3", "conv4"), 
    to_connection( "pool4", "conv5"), 
    to_connection( "pool5", "conv6"), 
    to_connection( "pool6", "conv7"), 
    to_connection( "pool7", "conv8"), 
    to_connection( "pool8", "conv9"), 


    to_end()
    ]

def main():
    namefile = str(sys.argv[0]).split('.')[0]
    to_generate(arch, namefile + '.tex' )

if __name__ == '__main__':
    main()
