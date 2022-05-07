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
