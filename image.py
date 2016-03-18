import Image, ImageFilter
import codecs
import Queue
import string,os,sys
import subprocess

#threshold = 160 #ios
threshold = 110 #android
dx=[0,1,0,-1]
dy=[1,0,-1,0]
exe='C:\\Program Files (x86)\\Tesseract-OCR\\tesseract.exe'
div=2

def roll(image, delta):
    xsize, ysize = image.size

    delta = delta % xsize
    if delta == 0: return image

    part1 = image.crop((0, 0, delta, ysize))
    part2 = image.crop((delta, 0, xsize, ysize))
    image.paste(part2, (0, 0, xsize-delta, ysize))
    image.paste(part1, (xsize-delta, 0, xsize, ysize))

    return image

def ocr(filename):
    print exe
    args = [exe, filename, 'out','-psm 7', 'pattern.txt']
    FNULL = open(os.devnull, 'w')
    proc = subprocess.Popen(args,stdout=FNULL, stderr=subprocess.STDOUT)
    retcode = proc.wait()
    FNULL.close()
    if retcode!=0:
        return ''
    f=open("out.txt", "r")
    return f.readline()
    
    
def readImage(filename):
    im = Image.open(filename)
    prefix=os.path.splitext(filename)[0]
    print prefix
    w,h=im.size
    if w>h:
        im=im.rotate(270)#TODO
    w,h=im.size
    if div>1: im=im.resize((w/div, h/div),Image.ANTIALIAS)
    w,h=im.size
    region = (w/6,h/3,w-1,h*3/4)
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
    #return ''
    #f=codecs.open("debug.txt", "w", 'utf-8')
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
                #print >>f,ymin,ymax,xmin,xmax,area,cnt
                if area>cnt*6 and area>best:
                    best=area
                    w1=ymin
                    w2=ymax
                    h1=xmin
                    h2=xmax
    print best,w1,w2,h1,h2
    #f.close()
    #bim = Lim.point(table, '1')
    bim=Lim
    if best>0:
        w=w2-w1
        h=h2-h1
        region = (w1+w/8,h1+h/5,w2-w/8,h2-h/6)
        tim=bim.crop(region)
        #tim.point(table, '1').show()
        #tim = tim.filter(ImageFilter.FIND_EDGES)  
        tim=tim.point(table, '1')
        
        pix=tim.load()
        w,h=tim.size
        for i in range(h/4):
            for j in range(w/10):
                pix[j,i]=1
        #tim.show()
        tim.save('final.jpg')
        #tim.save(prefix+'.number.jpg')
        #print image_to_string(tim)
        ret=ocr('final.jpg')
        print ret
        return ret
    else:
        #failed to find'
        print 'failed...'
    im.save('original.jpg')
    return ''
    
def readDir(dir,count=-1):
    files = os.listdir(dir)  
    fout=codecs.open(dir+os.sep+'serial.csv', "w", 'utf-8')
    id=1
    for f in files:
        if count==0: break
        path=dir+os.sep+f
        suf=os.path.splitext(path)[1].lower()
        if suf=='.jpg' or suf=='.png':
            ret=readImage(path)
            ret=ret.replace(' ',',',1)
            ret=ret.replace(' ','')
            ret=ret.strip('\n')
            print >>fout,`id`+','+ret+','+f
            count=count-1
            id=id+1

def printImage(filename):
    im = Image.open(filename)
    im=im.rotate(270)#TODO
    prefix=os.path.splitext(filename)[0]
    print prefix
    w,h=im.size
    #if div>1: im=im.resize((w/div, h/div),Image.ANTIALIAS)
    w,h=im.size
    #region = (w/6,h/3,w-1,h*3/4)
    #im=im.crop(region)
    
    Lim = im.convert('L')
    Lim.save('fun_gray.jpg')
    pix=Lim.load()
    f=codecs.open("debug.txt", "w", 'utf-8')
    for i in range(h):
        line=''
        for j in range(w):
            line=line+`pix[j,i]`+' '
        print >>f,line
    f.close()

#printImage('votes/IMG_6175.JPG')
    
#main
readImage('votes/IMG_6177.JPG')
#readImage('votes2/IMG_20160315_230347.JPG')
#readDir('votes2')
#printImage('votes/IMG_6173.JPG')