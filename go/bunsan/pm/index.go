package pm

import (
    "encoding/json"
    "fmt"
    "io/ioutil"
)

const (
    maxFileSize   = 64 * 1024 * 1024 // 64 MiB
    IndexFileName = "index"
)

type indexReadError struct {
    err error
}

func (e *indexReadError) Error() string {
    return fmt.Sprintf("unable to read index: %v", e.err)
}

type indexWriteError struct {
    err error
}

func (e *indexWriteError) Error() string {
    return fmt.Sprintf("unable to write index: %v", e.err)
}

type TreeImport struct {
    Path    string `json:"path"`
    Package string `json:"package"`
}

type LocalImport struct {
    Path   string `json:"path"`
    Source string `json:"source"`
}

type ExternalImports struct {
    Source  []TreeImport `json:"source"`
    Package []TreeImport `json:"package"`
}

type PackageStage struct {
    Import ExternalImports `json:"import"`
    Self   []LocalImport   `json:"self"`
}

func (p *PackageStage) Empty() bool {
    return len(p.Self) == 0 && len(p.Import.Source) == 0 && len(p.Import.Package) == 0
}

func (p *PackageStage) All() []string {
    all := make(map[string]struct{})
    for _, imp := range p.Import.Source {
        all[imp.Package] = struct{}{}
    }
    for _, imp := range p.Import.Package {
        all[imp.Package] = struct{}{}
    }
    result := make([]string, 0, len(all))
    for entry, _ := range all {
        result = append(result, entry)
    }
    return result
}

type Index struct {
    Source  PackageStage `json:"source"`
    Package PackageStage `json:"package"`
}

func (x *Index) All() []string {
    all := make(map[string]struct{})
    for _, entry := range x.Source.All() {
        all[entry] = struct{}{}
    }
    for _, entry := range x.Package.All() {
        all[entry] = struct{}{}
    }
    result := make([]string, 0, len(all))
    for entry, _ := range all {
        result = append(result, entry)
    }
    return result
}

func (x *Index) Sources() []string {
    sources := make(map[string]struct{})
    for _, imp := range x.Source.Self {
        sources[imp.Source] = struct{}{}
    }
    for _, imp := range x.Package.Self {
        sources[imp.Source] = struct{}{}
    }
    result := make([]string, 0, len(sources))
    for source, _ := range sources {
        result = append(result, source)
    }
    return result
}

func (x *Index) Unmarshal(data []byte) error {
    err := json.Unmarshal(data, x)
    if err != nil {
        return &indexReadError{err}
    }
    return nil
}

func (x *Index) Marshal() ([]byte, error) {
    data, err := json.MarshalIndent(x, "", "    ")
    if err != nil {
        return nil, &indexWriteError{err}
    }
    return data, nil
}

func (x *Index) ReadFromString(data string) error {
    return x.Unmarshal([]byte(data))
}

func (x *Index) ReadFromFile(filename string) error {
    data, err := ioutil.ReadFile(filename)
    if err != nil {
        return &indexReadError{err}
    }
    return x.Unmarshal(data)
}

func (x *Index) WriteToFile(filename string) error {
    data, err := x.Marshal()
    if err != nil {
        return err
    }
    err = ioutil.WriteFile(filename, data, 0666)
    if err != nil {
        return &indexWriteError{err}
    }
    return nil
}