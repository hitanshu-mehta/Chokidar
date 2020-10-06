# Intrusion-Detection-using-machine-learning-techniques
This is application is simplified intrusion detection system which using machine
learning techniques to detect malicious network traffic. It captures real-time network
traffic data, then it generates flows from this data. Training machine learning model
using flows rather than raw packet data greatly improves the accuracy. In addition to
that, it uses individual models for DoS, DDoS and port scan attacks. It also have one
model to detect abnormal traffic. This approach enables application to detect DoS,
DDoS and Port Scan with high accuracy and one extra model ensures that IDS also
detects unknown attacks. It also have minimal GUI displaying necessary information
to the user.

![](ui.png)

## Build
### Pre-requisites
* qt5 - [Installation guide](https://doc.qt.io/qt-5/gettingstarted.html#installing-qt)
* cmake - [Installation guide](https://cmake.org/install/)
* mongodb - [Download page](https://www.mongodb.com/download-center/community)
### Linux
```
git clone https://github.com/hitanshu-mehta/Intrusion-Detection-using-machine-learning-techniques.git
cd Intrusion-Detection-using-machine-learning-techniques
python3 -m build.py
```
New folder named build will be created in the parent directory.
```
build
├── bin
├── CMakeFiles
└── src
```
Executable file named IDS will be in the bin directory. Open terminal and run executable.
```
sudo ./IDS
```
Note that model folder should be in the same directory while running the executable. 

we need to run executable as root in order to capture traffic in [promiscuous mode](https://www.tcpdump.org/manpages/pcap.3pcap.html).
