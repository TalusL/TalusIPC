# TalusIPC
更符合中国人使用习惯的开源摄像机主程序。集成GB/T 28181、Onvif、RTSP、RTMP、WebRTC
* 整合 [divinus](https://github.com/TalusL/divinus/) 作为多款国产芯片方案摄像机HAL能力底座
* 整合[ZLMediaKit](https://github.com/ZLMediaKit/ZLMediaKit)作为音视频流媒体能力底座

## 芯片平台支持，详见[divinus](https://github.com/TalusL/divinus/)

| SoC Family              | Audio Stream | JPEG Snapshot | fMP4 Stream | RTSP Stream | On-Screen Display* |
|-------------------------|:------------:|:-------------:|:-----------:|:-----------:|:------------------:|
| AK3918                  | ↻            | ↻            | ↻           | ↻           | ↻                 |
| CV181x[^1]              | ↻            | ↻            | ↻           | ↻           | ↻                 |
| GM813x[^2]              | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3516AV100[^3]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3516CV100[^4]         | ↻            | ↻            | ✔️          | ↻           | ↻                 |
| Hi3516CV200[^5]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3516CV300[^6]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3516CV500[^7]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3516EV200[^8]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| GK7205V200[^9]          | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3519V100[^10]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3519AV100             | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| Hi3559AV100             | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| RV11xx[^11]             | ↻            | ✔️           | ✔️          | ✔️          | ↻                 |
| T31 series              | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| infinity3[^12]          | ↻            | ↻            | ↻           | ↻           | ↻                 |
| infinity6[^13]          | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| infinity6b0[^14]        | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| infinity6e[^15]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| infinity6c[^16]         | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |
| mercury6[^17]           | ✔️           | ✔️           | ✔️          | ✔️          | ✔️                |

_✔️ - supported, ↻ - in development, ✗ - unsupported, ⁿ/ₐ - not supported by hardware_


## 输出协议支持
- GB/T 28181 适配中
- Onvif 适配中
- RTSP 已适配
- RTMP 已适配
- WebRTC、WHIP、WHEP 已适配

## WebUI
- 监控视频实时预览 适配中
- PTZ操控 适配中
- ISP调节 适配中
- 录像回放 适配中
- 系统管理
  - 网络管理 适配中
  - 服务管理 适配中
  - 任务计划 适配中
- 接入协议
  - GB/T 28181 适配中
  - Onvif 适配中
  - RTMP、RTSP、WHIP、SRT推流 适配中