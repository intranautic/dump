#!/bin/sh
cd /opt/resolve/libs
sudo mkdir /opt/resolve/libs/_disabled
sudo mv libgio* libglib* libgmodule* libgobject* _disabled
