#include <iostream>
#include <string>
#include <CImg.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <dirent.h>
using namespace cimg_library;

using namespace std;

struct Point {
    int w,h;
    Point(int _w, int _h): w(_w),h(_h) {
    }
};

//length of the serial number
const int LEN1 = 8;
const int LEN2 = 8;

int s[10000][5000];//max height 10000 * width 5000
int s2[10000][5000];//max height 10000 * width 5000; 0-black, 1-white

int dx[] = {1,0,-1,0};
int dy[] = {0,1,0,-1};

string filename = "";
string folder = "";
int threshold = 0;

const string cmd = "tesseract";
const string args = "out -psm 7 pattern.txt 2>/dev/null";
const string finalname = "final.jpg";
const string cmd_line = cmd + " " + finalname + " " + args;

//threshold for gray to 2-value

string check()
{
    FILE *fin = fopen("out.txt", "r");
    char line[100];
    if (fin && fgets(line, 100, fin) != NULL) {
        //puts(line);
        int cnt = 0;
        string ret = "";
        int len = strlen(line);
        for (int i = 0; i < len; ++i) {
            if (isalnum(line[i])) {
                ret+=line[i];
                ++cnt;
            } else if (isspace(line[i])) {
                if (ret.size()==LEN1)
                    ret+=',';
            } else {
                printf("        OCR result: %s", line);
                return "";
            }
        }
        if (cnt != LEN1 + LEN2) {
                printf("        OCR result: %s", line);
            return "";
        }
        return ret;
    }
    return "";
}

string doit2(int w, int h, int threshold, const CImg<unsigned char>& img)
{
	for (int i = 0; i < h; ++i) {
	    for (int j = 0; j < w; ++j) {
	        if (s[i][j] < threshold)
	            s2[i][j]=0;//black
	        else
	            s2[i][j]=1;//white
	    }
	}
	
    CImg<unsigned char> bimg(img);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            float v = 0;
            if (s2[i][j]) v=255;
            bimg(j,i,0)=v;
            bimg(j,i,1)=v;
            bimg(j,i,2)=v;
        }
    }
   	bimg.save("binary.jpg");
	
	int tot = 0;
	double best = 0;
	int bw1=-1, bw2, bh1, bh2;
	for (int i = 0; i < h; ++i)
	    for (int j = 0; j < w; ++j) if (!s2[i][j]) {
	        queue<Point> q;
	        q.push(Point(j,i));
	        s2[j][i]=1;
            int cnt=1;
            int w1=j,w2=j,h1=i,h2=i;
	        while (!q.empty()) {
	            Point cur = q.front();
	            q.pop();
	            if (cur.w<w1) w1=cur.w;
	            if (cur.w>w2) w2=cur.w;
	            if (cur.h<h1) h1=cur.h;
	            if (cur.h>h2) h2=cur.h;

	            for (int d = 0; d < 4; ++d) {
	                if (cur.w+dx[d]<w && cur.w+dx[d]>=0 && cur.h+dy[d]<h && cur.h+dy[d]>=0 && !s2[cur.h+dy[d]][cur.w+dx[d]]) {
	                    s2[cur.h+dy[d]][cur.w+dx[d]]=1;
	                    q.push(Point(cur.w+dx[d],cur.h+dy[d]));
	                    ++cnt;
	                }
	            }
	        }
	        int area=(w2-w1+1)*(h2-h1+1);
	        double ratio = (double)(w2-w1+1)/(h2-h1+1);
	        double per = (double)area / (w*h);
	        double sparse = (double)area/cnt;
	        if (sparse < 6) continue;//should be sparse
	        if (per < 0.04) continue;//compare with WHOLE picture witout cropping
	        if (per > 0.3) continue;//compare with WHOLE picture witout cropping
	        if (ratio < 4) continue;//should be a LONG area
	        printf("        %d: cnt=%d area=%d ratio=%.2lf per=%.5lf sparse=%.2lf\n",tot,cnt,area,ratio,per,sparse);
	        if (sparse>best) {
	            best = sparse;
	            bw1=w1; bw2=w2;
	            bh1=h1; bh2=h2;
	        }
	        
	        CImg<unsigned char> img2(bimg);
	        img2.crop(w1,h1,0,0,w2,h2,0,2);
	        char tt[100]={0};
	        sprintf(tt,"part_%d.jpg", tot++);
        	img2.save(tt);
	        //img2.display(tt);
	    }
	if (bw1>=0) {
        CImg<unsigned char> img2(bimg);
        //CImg<unsigned char> img2(img);
        //img2.threshold(threshold);
        //img2.normalize(0,255);
        w=bw2-bw1+1;
        h=bh2-bh1+1;
	    img2.crop(bw1+w/8,bh1+h/5,0,0,bw2-w/8,bh2-h/6,0,2);
	    w=img2.width();
	    h=img2.height();
	    for (int i = 0; i < h/4; ++i)
	        for (int j = 0; j < w/10; ++j) {
	            img2(j,i,0)=255;
	            img2(j,i,1)=255;
	            img2(j,i,2)=255;
	        }
        img2.save(finalname.c_str());

        system(cmd_line.c_str());
        string ret = check();
        if (ret.size()==0) {
            printf("        OCR failed...\n");
        }
        return ret;
	} else {
	    printf("        no area found!\n");
	    return "";
	}
}

string doit(string filename)
{
    printf("Image file %s\n", filename.c_str());
	CImg<unsigned char> image(filename.c_str());
	if (image.width()>image.height())
	    image.rotate(90);
	image.resize_halfXY();
	int w=image.width();
	int h=image.height();
	//cout<<image.width()<<" "<<image.height()<<" "<<image.depth()<<" "<<image.spectrum()<<endl;
	
	image.crop(w/6,h/3,0,0,w-1,h-1-h/4,0,2);

	w=image.width();
	h=image.height();	
	
	for (int i = 0; i < h; ++i) {
	    for (int j = 0; j < w; ++j) {
	        float r = image(j,i,0,0);
	        float g = image(j,i,0,1);
	        float b = image(j,i,0,2);
	        float gray = (r*299 + g*587 + b*114 + 500) / 1000;
	        
	        s[i][j]=(int)gray;
	        
	        image(j,i,0)=gray;
	        image(j,i,1)=gray;
	        image(j,i,2)=gray;
	    }
	}
	
	//image.display("test1");
	image.save("gray.jpg");
	
	string ret = "";
	if (threshold>0) {
    	ret = doit2(w,h,threshold,image);
    	return ret;
    } else {
        int t=160;
        do {
            printf("    try threshold %d...\n", t);
            ret = doit2(w,h,t,image);
            if (ret.size() > 0) break;
            t-=20;
        } while (t >= 100);
    }
	return ret;
}

void doit_folder(string dirname)
{
    printf("check for dir %s\n", dirname.c_str());
    vector<string> vs;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirname.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            //printf ("%s\n", ent->d_name);
            string file(ent->d_name);
            if (file.size()>4) {
                string sf = file.substr(file.size()-3);
                if (sf=="jpg" || sf=="JPG")
                    vs.push_back(file);
            }
        }
        closedir (dir);
        sort(vs.begin(), vs.end());
        FILE *fout = fopen((dirname+"/serial.csv").c_str(), "w");
        for (int i = 0; i < vs.size(); ++i) {
            string ret = doit(dirname+'/'+vs[i]);
            if (ret.size()>0) {
                fprintf(fout, "%d,%s,,%s\n", i+1, ret.c_str(), vs[i].c_str());
                printf("result: %s\n", ret.c_str());
            } else {
                fprintf(fout, "%d,,,,%s\n", i+1, vs[i].c_str());
                printf("Not recognized...\n");
            }
        }
    } else {
        printf("Error reading dir %s\n", dirname.c_str());
    }
}

void help()
{
    puts("Usage: ./image [options]");
    puts("options:       -i image_filename       <specify one input image filename>");
    puts("               -f folder_name          <specify one folder (no trailing '/')>");
    puts("               -t threshold       <specify the threshold>");
    exit(0);
}

int main(int argc, char **argv)
{
    //freopen("debug.txt", "w", stdout);
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i],"-i")==0 && i+1<argc) {
            filename=argv[i+1];
            printf("input image filename: %s\n", filename.c_str());
            ++i;
        } else if (strcmp(argv[i],"-f")==0 && i+1<argc) {
            ++i;
            folder=argv[i];
            printf("folder: %s\n", folder.c_str());
        } else if (strcmp(argv[i],"-t")==0 && i+1<argc) {
            ++i;
            sscanf(argv[i], "%d", &threshold);
            printf("threshold=%d\n", threshold);
        } else {
            help();
        }
    }
    
    printf("cmd: %s\n", cmd_line.c_str());
    
    if (filename.size() > 0) {
        string ret = doit(filename);
        if (ret.size() > 0) {
            printf("result: %s\n", ret.c_str());
        } else {
            printf("Not recognized...\n");
        }
    } else if (folder.size() > 0) {
        doit_folder(folder);
    } else {
        help();
    }
}
