#include "decode.h"
#include <QDebug>
extern "C" {
    #include <libavutil/samplefmt.h>
    #include <libavutil/dict.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswresample/swresample.h>

}
Decode::Decode(QObject *parent) : QObject(parent)
{

}

bool Decode::decode(QString filePath, QString dirPath)
{
    AVFormatContext *pFormat=avformat_alloc_context();
    QByteArray temp=filePath.toUtf8();
    const char *path=temp.constData();
    QString out=dirPath+"/out.pcm";
    QByteArray ou=out.toUtf8();
    const char *output=ou.constData();
    int ret=avformat_open_input(&pFormat,path,NULL,NULL);
    if(ret!=0 || pFormat==NULL) {
        qDebug()<<"无法打开文件！";
        return 0;
    }
    //获取文件流信息
    if(avformat_find_stream_info(pFormat,NULL)<0) {
        qDebug()<<"获取流信息失败！";
        return 0;
    }
    int audio_stream_idx=-1;
    for(unsigned int i=0;i<pFormat->nb_streams;++i) {
        if(pFormat->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO) {
            audio_stream_idx=i;
            break;
        }
    }
    AVCodecParameters *codecpar=pFormat->streams[audio_stream_idx]->codecpar;
    //找到解码器
    AVCodec *dec=const_cast<AVCodec *>(avcodec_find_decoder(codecpar->codec_id));
    //创建上下文
    AVCodecContext *codecContext=avcodec_alloc_context3(dec);
    avcodec_parameters_to_context(codecContext,codecpar);
    codecContext->pkt_timebase = pFormat->streams[audio_stream_idx]->time_base;
    avcodec_open2(codecContext,dec,NULL);
    SwrContext *swrContext=swr_alloc();

    //输入的这些参数
    AVSampleFormat in_sample=codecContext->sample_fmt;
    //输入采样率
    int in_sample_rate=codecContext->sample_rate;
    //输入声道布局
    uint64_t in_ch_layout=codecContext->channel_layout;

    //输出参数
    //输出采样格式
    AVSampleFormat out_sample=AV_SAMPLE_FMT_S16;
    //输出采样率
    int out_sample_rate=44100;
    uint64_t out_ch_layout=AV_CH_LAYOUT_STEREO;

    swr_alloc_set_opts(swrContext,out_ch_layout,out_sample,
                       out_sample_rate,in_ch_layout,in_sample,in_sample_rate,0,NULL);

    swr_init(swrContext);
    uint8_t *out_buffer=(uint8_t *)(av_malloc(2*44100));
    FILE *fp_pcm=fopen(output,"wb");
    AVPacket *packet=av_packet_alloc();
    while(av_read_frame(pFormat,packet)>=0) {
        avcodec_send_packet(codecContext,packet);
        //解压缩数据
        AVFrame *frame=av_frame_alloc();
        int ret=avcodec_receive_frame(codecContext,frame);
        if(ret==AVERROR(EAGAIN)) {
            continue;
        } else if(ret<0) {
            qDebug()<<"解码完成！";
            break;
        }
        if(packet->stream_index!=audio_stream_idx) {
            continue;
        }
        swr_convert(swrContext,&out_buffer,2*44100,(const uint8_t **)frame->data,frame->nb_samples);
        int out_channerl_nb=av_get_channel_layout_nb_channels(out_ch_layout);
        int out_buffer_size=av_samples_get_buffer_size(NULL,out_channerl_nb,frame->nb_samples,out_sample,1);
        fwrite(out_buffer,1,out_buffer_size,fp_pcm);
    }
    getVertic(out);
    return true;
}

void Decode::getVertic(QString filePath)
{
    m_vertices.clear();
    short pcm_In=0;
    int size=0;
    int n = 0, m = 0;
    float num = 0.0;
    FILE *fp=fopen(filePath.toUtf8().data(),"rb+");
    while(!feof(fp)){
        if(n/8820 == m){
            if(n%8820 >= 0 && n%8820 < 34){
                size=fread(&pcm_In,2,1,fp);       //读取音频数据，1个数据两个字节
                if(size>0) {
                    num = pcm_In/500;
                    if(num < 0){
                        num = num*(-1);
                    }
                    m_vertices.push_back(num);
                    n++;
                }
            }else{
                m++;
                n = m*8820;
                fseek(fp, n*2, 0);
            }
        }
    }
    fclose(fp);
}

