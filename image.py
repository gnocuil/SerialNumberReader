import Image
import codecs
import Queue
import string,os,sys

threshold = 160
dx=[0,1,0,-1]
dy=[1,0,-1,0]

def roll(image, delta):
    xsize, ysize = image.size

    delta = delta % xsize
    if delta == 0: return image

    part1 = image.crop((0, 0, delta, ysize))
    part2 = image.crop((delta, 0, xsize, ysize))
    image.paste(part2, (0, 0, xsize-delta, ysize))
    image.paste(part1, (xsize-delta, 0, xsize, ysize))

    return image

def readImage(filename):
    im = Image.open(filename)
    prefix=os.path.splitext(filename)[0]
    print prefix
    im=im.rotate(270)#TODO
    w,h=im.size
    region = (w/6,h/2,w-1,h*3/4)
    im=im.crop(region)
    
    Lim = im.convert('L')
    Lim.save('fun_gray.jpg')
    
    table = []
    for i in range(256):
        if i < threshold:
            table.append(0)
        else:
            table.append(1)

    bim = Lim.point(table, '1')
    bim.save('fun_binary.jpg')
    
    f=codecs.open("debug.txt", "w", 'utf-8')
    w,h=bim.size
    print w,h
    pix=bim.load()
    #0-black, 1-white
    best=0
    w1=0
    w2=0
    h1=0
    h2=0
    for i in range(h):
        for j in range(w):
            if pix[j,i]==0:
                cnt=0
                xmax=i
                xmin=i
                ymax=j
                ymin=j
                q=Queue.Queue()
                q.put((j,i))
                pix[j,i]=1
                while not q.empty():
                    cnt=cnt+1
                    y,x=q.get()
                    if x<xmin: xmin=x
                    if y<ymin: ymin=y
                    if x>xmax: xmax=x
                    if y>ymax: ymax=y
                    for k in range(4):
                        x2=x+dx[k]
                        y2=y+dy[k]
                        if x2>=0 and x2<h and y2>=0 and y2<w and pix[y2,x2]==0:
                            q.put((y2,x2))
                            pix[y2,x2]=1
                area=(xmax-xmin+1)*(ymax-ymin+1)
                print >>f,ymin,ymax,xmin,xmax,area,cnt
                if area>cnt*6 and area>best:
                    best=area
                    w1=ymin
                    w2=ymax
                    h1=xmin
                    h2=xmax
    print best,w1,w2,h1,h2
    f.close()
    bim = Lim.point(table, '1')
    if best>0:
        region = (w1,h1,w2,h2)
        tim=bim.crop(region)
        tim.show()
        tim.save('final.jpg')
    else:
        #failed to find'
        print 'failed...'
    im.save('original.jpg')
    
def readDir(dir,count=-1):
    files = os.listdir(dir)  
    for f in files:
        if count==0: break
        path=dir+os.sep+f
        suf=os.path.splitext(path)[1].lower()
        if suf=='.jpg' or suf=='.png':
            readImage(path)
            count=count-1
    
#main
#readImage('votes/IMG_6176.JPG')
readDir('votes')