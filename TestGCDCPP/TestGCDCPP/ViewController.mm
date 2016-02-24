//
//  ViewController.m
//  TestGCDCPP
//
//  Created by ximiao on 16/1/20.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#import "ViewController.h"
#import "XDispatch.h"
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "GLView.h"
#include <glm/glm.hpp>
#include <XString.hpp>
#include <XImage.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "XHTTPClient.h"


using namespace glm;

using namespace XDispatch;
int totalNumber(0), number1(0), number2(0);
void test() {
    srand(5246);
    clock_t t1 = clock();
    //std::atomic_int_fast64_t totalNumber(0), number1(0), number2(0);
    int i = 0;
    
    auto queue1 = XTaskQueue::getMainQueue();
    XDispatchManager* manager = XDispatchManager::getSharedInstance();
    for (; i < 1000000; ++i)
    {
        int ms = rand() % 2000;
        if (ms > 0)
        {
//            manager->dispatchAsnyc(queue1, []{
//                //for (int i = 0; i < 1000; ++i)
//                {
//                    ++totalNumber;
//                    ++number1;
//                }
//            });
            dispatch_async(dispatch_get_main_queue(), ^{
                ++totalNumber;
                ++number1;
            });
        }
        else
        {
//            XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getGlobleQueue(XTaskPriority_High), [&totalNumber, &number2]{
//                {
//                    ++totalNumber;
//                    ++number2;
//                }
//            }, ms);
//            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(ms * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
//                ++totalNumber;
//                ++number2;
//            });
        }
    }
    clock_t t2 = clock();
    std::cout << "count:" << i << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
    std::cout << "total:" << totalNumber << "   number1:" << number1 << "   number2:" << number2 << std::endl;
}

struct XColor {
    char r;
    char g;
    char b;
    char a;
};

struct XCamara {
    char eye_x = '\0';
    char eye_y = '\0';
    char eye_z = '\1';
    char t_1 = '\0';
};

GLfloat texturePos[] = {
    0.00, 0.00,
    1, 0,
    0, 1,
    1, 1
};

GLfloat square[] = {
    0, 0, 0.5,
    300, 0, 0.5,
    0, 300, 0.5,
    300, 200, 0.5};

GLfloat transformMat[] = {
    1.0f/150.0f, 0, 0, 0,
    0, 1.0f/150.0f, 0, 0,
    0, 0, 1, 0,
    -1, -1, 0, 1,};

std::vector<GLfloat> vectors;
std::vector<GLuint> textureNum;
std::vector<GLfloat> textureVectors;
std::vector<XColor> colors;

template<class T> struct must_be_pod

{
    
    union
    
    {
        
        T noname;
        
    };
    
};

int gifIndex;

std::map<int, int>textureMap;

class GLTextureManager {
    GLTextureManager &sharedInstanc() {
        static GLTextureManager manager;
        return manager;
    }
    
};

GLuint loadTexture(NSString *text) {
    static XImage image("/Users/ximiao/Desktop/TestGCDCPP/TestGCDCPP/test.jpg");
    GLuint textureId;
    auto iter = textureMap.find(gifIndex);
    if (iter == textureMap.end()) {
        NSUInteger width = 0;
        NSUInteger height = 0;
        //    UIImage *image = [UIImage imageNamed:@"test.jpg"];
        //    CGImageRef imageRef = [image CGImage];
        //    NSUInteger width = CGImageGetWidth(imageRef);
        //    NSUInteger height = CGImageGetHeight(imageRef);
        //    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        //    XColor *rawData = new XColor[height * width];
        //    NSUInteger bytesPerPixel = 4;
        //    NSUInteger bytesPerRow = bytesPerPixel * width;
        //    NSUInteger bitsPerComponent = 8;
        //    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
        //                                                 bitsPerComponent, bytesPerRow, colorSpace,
        //                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        //    CGColorSpaceRelease(colorSpace);
        //
        //    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
        //    CGContextRelease(context);
        width = image.width();
        height = image.height();
        XColor *rawData = new XColor[width * height];
        image.getImage(rawData, gifIndex);
        
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawData);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        textureMap[gifIndex] = textureId;
    } else {
        textureId = iter->second;
    }
    ++gifIndex;
    if (gifIndex >= image.count()) {
        gifIndex = 0;
    }
    return textureId;
}

#define kDegreesToRadian(x) (M_PI * (x) / 180.0)
#define kRadianToDegrees(radian) (radian*180.0)/(M_PI)

@interface ViewController ()
@property (nonatomic, strong)UIImageView *imageView;
@property (nonatomic, strong)UIImageView *imageView2;
@property (nonatomic, strong)UIImageView *imageView3;
@property (nonatomic, strong)UIImageView *imageView4;
@property (nonatomic, strong)UIImageView *imageView5;
@end

@implementation ViewController {
    int width;
    int height;
    GLuint program;
    GLuint framebuffer;
    GLuint vectexArrayObject;
    XColor *pixels;
    
    GLView *glView;
    EAGLContext *context;
    
    FT_Library  library;   /* handle to library     */
    FT_Face     face;      /* handle to face object */
}

- (UIImageView *)imageView {
    if (!_imageView) {
        _imageView = [[UIImageView alloc] initWithFrame:CGRectMake(10, 350, 200, 200)];
        [self.view addSubview:_imageView];
    }
    return _imageView;
}
- (UIImageView *)imageView2 {
    if (!_imageView2) {
        _imageView2 = [[UIImageView alloc] initWithFrame:CGRectMake(220, 350, 20, 20)];
        [self.view addSubview:_imageView2];
    }
    return _imageView2;
}
- (UIImageView *)imageView3 {
    if (!_imageView3) {
        _imageView3 = [[UIImageView alloc] initWithFrame:CGRectMake(250, 350, 20, 20)];
        [self.view addSubview:_imageView3];
    }
    return _imageView3;
}
- (UIImageView *)imageView4 {
    if (!_imageView4) {
        _imageView4 = [[UIImageView alloc] initWithFrame:CGRectMake(220, 380, 20, 20)];
        [self.view addSubview:_imageView4];
    }
    return _imageView4;
}
- (UIImageView *)imageView5 {
    if (!_imageView5) {
        _imageView5 = [[UIImageView alloc] initWithFrame:CGRectMake(250, 380, 20, 20)];
        [self.view addSubview:_imageView5];
    }
    return _imageView5;
}

- (void)InitFreeType {
    FT_Error error = FT_Init_FreeType( &library );
    if (error)
    {
        return;
    }
    error = FT_New_Face( library,
                        "/Users/ximiao/Downloads/FZMWFont.ttf",
                        0,
                        &face );
    if ( error == FT_Err_Unknown_File_Format )
    {
        return;
    }
    else if ( error )
    {
        return;
    }
    
    
//    error = FT_Set_Char_Size(
//                             face,    /* handle to face object           */
//                             0,       /* char_width in 1/64th of points  */
//                             16*64,   /* char_height in 1/64th of points */
//                             300,     /* horizontal device resolution    */
//                             300 );   /* vertical device resolution      */
    error = FT_Set_Pixel_Sizes(
                               face,   /* handle to face object */
                               0,      /* pixel_width           */
                               300 );   /* pixel_height          */
    
}

- (void)drawFreeType {
    XString string("测试");
    FT_UInt glyph_index = FT_Get_Char_Index( face, string.getUnicodeString()[1] );
    //If no glyph, return 0, but not error, it can show space.
    int error = FT_Load_Glyph(
                              face,          /* handle to face object */
                              glyph_index,   /* glyph index           */
                              FT_LOAD_DEFAULT );  /* load flags, see below */
    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        error = FT_Render_Glyph( face->glyph,   /* glyph slot  */
                                FT_RENDER_MODE_NORMAL ); /* render mode */
        if(error) {
            return;
        }
    }
    
    const FT_Bitmap *bitMap = &(face->glyph->bitmap);
    if (bitMap->pixel_mode == FT_PIXEL_MODE_GRAY) {
        CGDataProvider *provider = CGDataProviderCreateWithData(NULL, bitMap->buffer, bitMap->width * bitMap->rows, NULL);
        CGImage *cgImage = CGImageCreate(bitMap->width, bitMap->rows, 8, 8, bitMap->width, CGColorSpaceCreateDeviceGray(), kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);
        UIImage *image = [UIImage imageWithCGImage:cgImage];
        CGImageRelease(cgImage);
        CGDataProviderRelease(provider);
        self.imageView.image = image;
        [self.view setNeedsDisplay];
    }
}

- (void)testDraw {
    int s = sizeof(XColor);
//    GLuint frameBuffer = 100;
//    glGenFramebuffers(1, &frameBuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    [EAGLContext setCurrentContext:context];
    
    glView = [[GLView alloc] initWithFrame:CGRectMake(0, 0, width, height)];
    [self.view addSubview:glView];
    glView.backgroundColor = [UIColor clearColor];
    
    
    const GLfloat  zNear = 2.2;
    const GLfloat  zFar = 30000;
    const GLfloat  fieldOfView = 60.0;
    GLfloat size = zNear * tanf(kDegreesToRadian(fieldOfView) / 2.0);
    
    //parameters are: left, right, bottom, top, near, far
    
    
    GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)glView.layer];
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, width, height);
    //多重采样
    //glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_SRGB8_ALPHA8, width, height);
    
    GLenum err = glGetError();
    
//    //创建一个OpenGL ES用的无符号整形缓存,名为depthRenderbuffer
//    GLuint depthRenderbuffer;
//    //从depthRenderbuffer的地址起，分配一个渲染缓存的内存，（1个，地址）
//    glGenRenderbuffers(1, &depthRenderbuffer);
//    //把depthRenderbuffer绑定在OpenGL ES的渲染缓存GL_RENDERBUFFER上
//    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
//    //设定渲染缓存的存储类型（GL_DEPTH_COMPONENT16、宽、高）
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 100, 100);
//    //把GL_RENDERBUFFER里的depthRenderbuffer附在GL_FRAMEBUFFER的GL_DEPTH_ATTACHMENT（深度附着点）上
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
//    err = glGetError();
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    
    //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //如果状态不完全
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        //显示语句：创建完全的帧缓存对象失败，状态是...
        NSLog(@"failed to make complete framebuffer object %x", status);
    }
    
    
    glViewport(0, 0, width, height);
    
    glClearColor(0, 1, 0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    
    // Read vertex shader source
    NSString *vertexShaderSource = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"VertexShader" ofType:@"vsh"] encoding:NSUTF8StringEncoding error:nil];
    const char *vertexShaderSourceCString = [vertexShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
    
    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCString, NULL);
    glCompileShader(vertexShader);
    
    
    GLint compileResult;
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compileResult);
    if (GL_FALSE == compileResult)
    {
        int a = 1;
    }
    
    // Read fragment shader source
    NSString *fragmentShaderSource = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"FragmentShader" ofType:@"fsh"] encoding:NSUTF8StringEncoding error:nil];
    const char *fragmentShaderSourceCString = [fragmentShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
    
    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCString, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&compileResult);
    if (GL_FALSE == compileResult)
    {
        GLint logLen;
        //得到编译日志长度
        glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&logLen);
        if (logLen > 0)
        {
            char *log = (char *)malloc(logLen);
            GLsizei written;
            //得到日志信息并输出
            glGetShaderInfoLog(fragmentShader,logLen,&written,log);
            NSLog(@"Log:%s", log);
            free(log);//释放空间
        }
    }
    
    // Create and link program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &compileResult);
    if (GL_FALSE == compileResult) {
        GLint logLen;
        //得到编译日志长度
        glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&logLen);
        if (logLen > 0)
        {
            char *log = (char *)malloc(logLen);
            GLsizei written;
            //得到日志信息并输出
            glGetShaderInfoLog(fragmentShader,logLen,&written,log);
            NSLog(@"Log:%s", log);
            free(log);//释放空间
        }
    }
    
    // Use program
    glUseProgram(program);

    //Send geometry to vertex shader
    //const char *aPositionCString = [@"a_position" cStringUsingEncoding:NSUTF8StringEncoding];
    //GLuint aPosition = glGetAttribLocation(program, aPositionCString);
    
    GLuint bufObjects[2];
    glGenBuffers(2, bufObjects);
    
    glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturePos), texturePos, GL_STATIC_DRAW);
    
    
    glGenVertexArrays(1, &vectexArrayObject);
    glBindVertexArray(vectexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    [self play];
}

- (void)play {
    GLint location = glGetUniformLocation(program, "s_texture");
    GLuint textureId = loadTexture(@"test");
    
    glActiveTexture(GL_ACTIVE_TEXTURE - 1);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(location, GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
    
    glUniformMatrix4fv(0, 1, true, (GLfloat*)&transformMat);
    
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glBindVertexArray(vectexArrayObject);
    
    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    GLuint error = glGetError();
    
    //[context presentRenderbuffer:GL_RENDERBUFFER];
    
//    glBindFramebuffer(GL_FRAMEBUFFER,0);
//    //使用多重采样，所以不能直接读取多重采样renderbuffer中的数据，必须创建一个普通的
//    GLuint renderBuffer2;
//    glGenRenderbuffers(1, &renderBuffer2);
//    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer2);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, width, height);
//    GLuint framebuffer2;
//    glGenFramebuffers(1, &framebuffer2);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, framebuffer2);
//    
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
//    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT,GL_LINEAR);
//    glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    
    
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
    
    
    //pixels = new XColor[width * height];
//    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//    
//    CGDataProvider *provider = CGDataProviderCreateWithData(NULL, pixels, width * height * 4, NULL);
//    CGImage *cgImage = CGImageCreate(width, height, 8, 32, 4 * width, CGColorSpaceCreateDeviceRGB(), kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);
//    UIImage *image = [UIImage imageWithCGImage:cgImage];
//    CGImageRelease(cgImage);
//    CGDataProviderRelease(provider);
//    self.imageView.image = image;
//    [self.view setNeedsDisplay];
    
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)showImage:(UIImage*)img {
    dispatch_async(dispatch_get_main_queue(), ^{
        //self.imageView.image = img;
    });
}
- (void)showImage2:(UIImage*)img {
    dispatch_async(dispatch_get_main_queue(), ^{
        self.imageView2.image = img;
    });
}
- (void)showImage3:(UIImage*)img {
    dispatch_async(dispatch_get_main_queue(), ^{
        self.imageView3.image = img;
    });
}
- (void)showImage4:(UIImage*)img {
    dispatch_async(dispatch_get_main_queue(), ^{
        self.imageView4.image = img;
    });
}
- (void)showImage5:(UIImage*)img {
    dispatch_async(dispatch_get_main_queue(), ^{
        self.imageView5.image = img;
    });
}
std::shared_ptr<XHTTPRequest> request(new XHTTPRequest());
std::shared_ptr<XHTTPRequest> request2(new XHTTPRequest());
std::shared_ptr<XHTTPRequest> request3(new XHTTPRequest());
std::shared_ptr<XHTTPRequest> request4(new XHTTPRequest());
std::shared_ptr<XHTTPRequest> request5(new XHTTPRequest());
- (void)viewDidLoad {
    XThreadPool::initGlobelPool();
    [super viewDidLoad];
    
    request->url = "http://pic27.nipic.com/20130327/12143305_143942699176_2.jpg";
    request->finishCallBack = [&self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request);
    
    request->url = "http://c.hiphotos.baidu.com/zhidao/wh%3D450%2C600/sign=a22a60e952e736d15846840cae6063f4/6c224f4a20a4462313d38b579e22720e0cf3d7ed.jpg";
    request->finishCallBack = [self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request);
    
    request2->url = "http://e.hiphotos.baidu.com/zhidao/pic/item/d058ccbf6c81800afdde791bb33533fa828b47e8.jpg";
    request2->finishCallBack = [self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage2:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request2);
    
    request3->url = "http://pic23.nipic.com/20120822/2256974_142258905000_2.jpg";
    request3->finishCallBack = [self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage3:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request3);
    
    request4->url = "http://a.hiphotos.baidu.com/zhidao/pic/item/9358d109b3de9c82391372de6d81800a19d84369.jpg";
    request4->finishCallBack = [self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage4:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request4);
    
    request5->url = "http://b.hiphotos.baidu.com/zhidao/pic/item/8ad4b31c8701a18bbebefe219c2f07082838fe03.jpg";
    request5->finishCallBack = [self](std::shared_ptr<XHTTPResponse> response) {
        NSData *data = [NSData dataWithBytes:(void *)(response->buf()) length:response->bufSize()];
        UIImage *image = [UIImage imageWithData:data];
        [self showImage5:image];
    };
    XHTTPClient::getSharedInstanc()->sendRequest(request5);
    
    
    gifIndex = 0;
    width = 300;
    height = 300;
    UIViewController *c = [[UIViewController alloc] init];
    [self testDraw];
    [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(play) userInfo:nil repeats:YES];
    
    [self InitFreeType];
    [self drawFreeType];
    
    // Do any additional setup after loading the view, typically from a nib.
    //XThreadPool::initGlobelPool();
//    XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
//        std::cout << "test1";
//        XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
//            std::cout << "test2";
//            XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
//                std::cout << "test3";
//                
//            }, 3000);
//        }, 1000);
//    }, 600);
    //test();
    //[self testDraw];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
