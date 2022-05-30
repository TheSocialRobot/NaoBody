# NaoBody

The Social Robot running on a NAO robot

## qibuild setup

```bash
pyenv virtualenv 3.9.6 the-social-robot
pyenv activate the-social-robot
python3.9 -m pip install --upgrade pip
pip install qibuild

# create a worktree
qibuild init

# create a project
qisrc create body
qibuild configure body
qibuild make body
```

## C++ toolchain

For desktop

```bash
wget https://the-social-robot.s3.eu-west-2.amazonaws.com/nao-2.1.4.13/naoqi-sdk-2.1.4.13-linux64.tar.gz
qitoolchain create naoqi-sdk /path/to/cpp/sdk/toolchain.xml
qibuild add-config naoqi-sdk --toolchain naoqi-sdk
qibuild configure -c naoqi-sdk body
qibuild make -c naoqi-sdk body
```

For robot

```bash
wget https://the-social-robot.s3.eu-west-2.amazonaws.com/nao-2.1.4.13/ctc-linux64-atom-2.1.4.13.zip
qitoolchain create cross-atom /path/to/ctc/toolchain.xml
qibuild add-config cross-atom --toolchain cross-atom
qibuild configure -c cross-atom body
qibuild make -c cross-atom body
```

## SDKs

Currently available from aldebaran.com

Downloads page: [https://www.softbankrobotics.com/emea/en/support/nao-6/downloads-softwares/](https://www.softbankrobotics.com/emea/en/support/nao-6/downloads-softwares/)

- C++ [https://community-static.aldebaran.com/resources/2.1.4.13/sdk-c%2B%2B/naoqi-sdk-2.1.4.13-linux64.tar.gz](https://community-static.aldebaran.com/resources/2.1.4.13/sdk-c%2B%2B/naoqi-sdk-2.1.4.13-linux64.tar.gz)
- Python [https://community-static.aldebaran.com/resources/2.1.4.13/sdk-python/pynaoqi-python2.7-2.1.4.13-linux64.tar.gz](https://community-static.aldebaran.com/resources/2.1.4.13/sdk-python/pynaoqi-python2.7-2.1.4.13-linux64.tar.gz)
