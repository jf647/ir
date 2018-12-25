#include "flatb/matrix_descriptor_generated.h"
#include "persistent_matrix.h"
#include <fstream>
#include <mio/mmap.hpp>
#include <sys/mman.h>
#include <sys/stat.h>

static inline bool check_path(std::string path) {
  struct stat buffer;
  return stat(path.c_str(), &buffer) == 0;
}
static inline mio::ummap_sink load_mmaped_sink(std::string filename) {
  mio::ummap_sink sink;
  std::error_code error;
  sink.map(filename, error);
  if (error) {
    throw StoreException(error.message());
  }
  return sink;
}
struct PersistentMatrixBase::Store {
  // mio::basic_mmap_source<double> source;
  double *source;
  size_t mapping_size;
  Store(std::string index_data, size_t offset) {
    struct stat s;
    int status = stat(index_data.c_str(), &s);
    mapping_size = s.st_size;
    refresh(index_data, mapping_size);
  }
  void refresh(std::string index_data, size_t msize) {
    int fd = open(index_data.c_str(), O_RDONLY);
    source = (double *)mmap(NULL, msize, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
  }
  ~Store() { munmap(source, mapping_size); }
};

struct PersistentMatrixBase::Header {
  mio::ummap_sink h_sink;
  nvrdb::MatrixHeader *mh;

  Header(std::string index)
      : h_sink(load_mmaped_sink(index)),
        mh(flatbuffers::GetMutableRoot<nvrdb::MatrixHeader>(h_sink.data())) {}

  std::string name() const { return mh->descriptor()->name()->str(); }

  order matrix_order() const {
    return mh->order() == nvrdb::Order_ROW ? ROW : COL;
  }

  int primarySize() const { return mh->primarySize(); }
  int secondarySize() const { return mh->secondarySize(); }
  int incrementSecondary() {
    mh->mutate_secondarySize(1 + mh->secondarySize());
    std::error_code error;
    h_sink.sync(error);
    return mh->secondarySize();
  }
  int buffer() const { return mh->buffered(); }
  ~Header() { h_sink.unmap(); }
  size_t offset() { return primarySize() * secondarySize() * sizeof(double); }
  std::unique_ptr<PersistentMatrixBase::Store>
  load_store(std::string storefile) {
    return std::unique_ptr<PersistentMatrixBase::Store>(
        new PersistentMatrixBase::Store(storefile, offset()));
  }
};
void create_header(std::string index_path, std::string name, int primarySize,
                   order o) {
  flatbuffers::FlatBufferBuilder builder;
  builder.ForceDefaults(true);
  auto n = builder.CreateString(name);
  auto id = nvrdb::CreateIndexDescriptor(builder, n, 0);
  auto order = nvrdb::Order_ROW;
  if (o == COL)
    order = nvrdb::Order_COL;
  auto mh = nvrdb::CreateMatrixHeader(builder, id, order, primarySize);
  builder.Finish(mh);
  std::ofstream fp;
  fp.open(index_path, std::ios::out | std::ios::binary);
  fp.write((char *)builder.GetBufferPointer(), builder.GetSize());
  fp.flush();
  fp.close();
}
static inline std::string data_path(std::string path) {
  return path + "/.data";
}
static inline std::string header_path(std::string path) {
  return path + "/.index";
}

PersistentMatrixBase::PersistentMatrixBase(std::string path)
    : path(path), header(new PersistentMatrixBase::Header(header_path(path))),
      store(header->load_store(data_path(path))) {}

PersistentMatrixBase::PersistentMatrixBase(PersistentMatrixBase &&other) =
    default;

PersistentMatrixBase::~PersistentMatrixBase() {}

double *PersistentMatrixBase::data() {
  store->refresh(data_path(path), header->offset());
  return store->source;
}

int PersistentMatrixBase::write(std::vector<double> vec) {
  assert(header->primarySize() == vec.size());
  size_t location = (header->secondarySize() + header->buffer()) *
                    sizeof(double) * header->primarySize();
  std::ofstream bufwrite;
  bufwrite.open(data_path(path), std::ios::app);
  assert(bufwrite.tellp() == location);
  bufwrite.seekp(location);
  bufwrite.write(reinterpret_cast<char *>(vec.data()),
                 sizeof(double) * vec.size());
  header->incrementSecondary();
  return location;
}
int PersistentMatrixBase::primarySize() { return header->primarySize(); }
int PersistentMatrixBase::secondarySize() { return header->secondarySize(); }

PersistentMatrixBase PersistentMatrixBase::create(std::string name,
                                                  std::string path,
                                                  int primarySize, order o) {
  if (!check_path(path)) {
    if (mkdir(path.c_str(), 0777) == -1)
      throw StoreException(path + " base does not exist");
  } else {
    throw StoreException(path + "already exists");
  }
  create_header(header_path(path), name, primarySize, o);
  return PersistentMatrixBase(path);
}
