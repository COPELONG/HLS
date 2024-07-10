#  HLS服务器demo

![image-20240404170615988](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20240404170615988.png)

![image-20240404170851955](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20240404170851955.png)

读取m3u8索引文件发送

读取index文件发送

![image-20240328171505501](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20240328171505501.png)

![image-20231212202757620](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20231212202757620.png)

解析每一行，找到Get请求，并且解析出url，然后进行文件名拼接进行读取文件操作

根据url对比：

发送m3u8文件

![image-20231212202903380](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20231212202903380.png)

发送索引文件

![image-20231212202935745](https://my-figures.oss-cn-beijing.aliyuncs.com/Figures/image-20231212202935745.png)

```c++
send(mClientFd, http_headers, http_headers_len, 0);
send(mClientFd, buf, bufLen, 0);
```