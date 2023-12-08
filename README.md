
## **Dependencies**

### **Python**

We assume Python version 3.12. The list of Python packages can be installed via

```cmd
python -m pip install -r requirements.txt   
```

### **Lark**
Used in: all

For HCSP code translation and generation, we use Lark to build our parser. Documentation of lark can be found at their official guide: https://lark-parser.readthedocs.io/en/stable/ (lark should be installed automatically using requirements.txt).

### **Node**
Used in: hhlpy, hcsp_simulator

For GUI of hhlpy and hcsp_simulator, we use JavaScript to write frontend, so Node is needed. You may install Node by direction of their official guide: https://nodejs.org/

### **Flask**
Used in: hhlpy, hcsp_simulator

For the backend of hcsp_simulator, we use Flask to write the backend. Documentation for Flask can be found at their official guide: https://flask.palletsprojects.com/en/3.0.x/

### **Isabelle**
Used in: HHLProver

HHLProver is an extension of Isabelle,so you may install Isabelle2023 from:
https://isabelle.in.tum.de/, for details, see [HHLProver_README](./HHLProver/README.md)

### **Ubuntu 20.04**
Used in: hcsp2c

In hcsp2c, certain package for compile is only in Linux, so Linux system is need.
For Windows users, wsl2.0 may make things easier, see https://learn.microsoft.com/en-us/windows/wsl/install

In Ubuntu, gcc needs to be installed with cmd
```cmd
sudo apt install gcc
```

## **Tool chain Usages**
### **AADL2HCSP**


To translate AADL to HCSP, first run aadl2json.py in folder "Mars2.0/tools", name of startfile, config file and the dir folder path of them needs to be specified as arguments, for example, 

```cmd
python tools/aadl2json.py -d Examples/AADL/CCS/AADL -sf joint_model_nobus.aadl -cf config.json  
```
and the output JSON file is under the dir folder path, then translate it to HCSP by running json2hcsp.py in folder "Mars2.0/tools", name of JSON file and the output dir needs to be specified as arguments, for example,

```cmd
python tools/json2hcsp.py -jf Examples/AADL/CCS/AADL/joint_model_nobus.json -od Examples/AADL/CCS/TCS/generatedcode_nobus
```
Note: for model with multipule files that consists of usage "import", path of imported HCSP module needs to be written in ss2hcsp/import_path.txt, you may create this file and maintain it manually

### **Simulink/Stateflow2HCSP**

To tranlate S/S model to HCSP, first you need to convert S/S model(mdl or xls file) to .xml file, matlab provide following cmd:

```Matlab
save_system('*.mdl', '*.xml','ExportToXML', true)
```
hint: when running this cmd, model needs to be opened in simulink

and then you can use ssxml2hcsp.py in folder "Mars2.0/tools" to translate xml file to HCSP file. Input and output dir needs to be specified as arguments, for example,

```cmd
python tools/ssxml2hcsp.py -in Examples/Simulink/LunarLander.xml -out Examples/Simulink/LunarLander.txt
```

### **HCSP simulator**
Our HCSP simulator includes frontend and backend.
To open Frontend, first get into the folder "hcsp_simulator" by 
 ```cmd
cd hcsp_simulator
```
and run npm by
 ```cmd
npm start
```
Note: you may re-build the project before use, by getting in the folder and run npm build, the guide (generated automatically by Node) is [hcsp_simulator_README](./hcsp_simulator/README.md)

### **Formally verification**
We provide 2 ways of formally verification
1) hhlpy, based on Wolfram Engine, and the usage of it see [hhlpy_README](./hhlpy/README.md)
2) HHLprover+ Isabelle, usage of it see [HHLProver_README](./HHLProver/README.md)

### **HCSP2C**

To turn HCSP to C file, first run hcsp2Ccode.py in folder "Mars2.0/tools", for example,

```cmd
python tools/hcsp2Ccode.py -sf using/output_files/TCS/hcsp/systemv2.txt -dd using/output_files/TCS/Ccode -df systemv2 -step 1e-4 -mt 40.0
```

and then copy the generated C code, Mars2.0/hcsp2c/target/hcsp2c.h ,Mars2.0/hcsp2c/target/hcsp2c.c and compile.sh to linux (in the same folder), and run  

```cmd
bash compile.sh
```

then run the outputfile, you may use '>' to put outputs into a txt file

```cmd
xx.output > xx.txt
```
