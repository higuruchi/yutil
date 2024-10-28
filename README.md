# yutil

## Usage

```
yutil [SUB_COMMAND] [OPTION...]
```

### version creation

ファイルのバージョンを作成する．

```bash
$ yutil vc [OPTION...]
```

**option**

-p, --path		ファイルパス

**example**

```bash
$ yutil vc --path=hoge.txt
```

### overwriting

```bash
$ yutil ow [OPTION...]
```

**option**

-p, --path		ファイルパス  
-a, --arg			上書きを行う内容  
-v, --vno			バージョンIDを指定  
-o, --parent	親バージョンを開く  

**example**

```bash
$ yutil ow --path=hoge.txt --arg=fugafuga
```

### cat

バージョンの内容を表示する，

```bash
$ yutil cat [OPTION...]
```

**option**

-p, --path		ファイルパス  
-v, --vno			バージョンIDを指定  
-o, --parent	親バージョンを開く  

**example**

```bash
$ yutil cat --path=hoge.txt 
```

### versin list

親バージョン，子バージョンの一覧を表示する．

```bash
$ yutil dent [OPTION...]
```

**option**

-p, --path		ファイルパス  
-v, --vno			バージョンIDを指定  
-o, --parent	親バージョンを開く  

**example**

```bash
$ yutil dent --path=hoge.txt
```

### version deletion

指定したバージョンを削除する．


```bash
$ yutil delete [OPTION...]
```

**option**

-p, --path		ファイルパス  
-v, --vno			バージョンIDを指定  
-o, --parent	親バージョンを開く  

**example**

```bash
$ yutil delete --path=hoge.txt --vno=13
```

### version link

指定したバージョンにハードリンクを作成する，

```bash
$ yutil vln [OPTION...]
```

**option**

-p, --path		ファイルパス  
-v, --vno			バージョンIDを指定  
-o, --parent	親バージョンを開く  
-d, --target	リンク先のファイルパス  

**example**

```bash
$ yutil vln --path=hoge.txt --vno=13 --target=fuga.txt
```

## Build

```
make
```

