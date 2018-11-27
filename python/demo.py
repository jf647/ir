from os import listdir
from os.path import isfile, join
import grpc
import click

from proto.document import document_pb2
from proto.document import document_pb2_grpc
from proto.admin import admin_pb2
from proto.admin import admin_pb2_grpc
from proto.search import search_pb2
from proto.search import search_pb2_grpc

def list_files(basedir):
    onlyfiles = [f for f in listdir(basedir) if isfile(join(basedir, f))]
    return onlyfiles

def create_document_from_file(basedir, fname):
    idstr = fname
    with open(join(basedir,fname)) as f:
        content = f.read()
    field = document_pb2.Field(field_name="textField",text_field=document_pb2.TextField(values=[content]))
    doc= document_pb2.Document(id=idstr)
    doc.fields.extend([field])
    return idstr, doc

@click.command()
@click.argument('basedir', type=click.Path(exists=True))
def index(basedir):
    channel = grpc.insecure_channel('localhost:50051')
    stub = document_pb2_grpc.DocumentServiceStub(channel)
    for f in list_files(basedir):
        idstr, doc = create_document_from_file(basedir,f)
        response = stub.Put(document_pb2.IndexRequest(document=doc))
    click.echo("Done indexing")
    return

@click.command()
def create_index():
    channel = grpc.insecure_channel('localhost:50051')
    stub = admin_pb2_grpc.AdminServiceStub(channel)
    req = admin_pb2.CreateIndexRequest()
    req.index_schema.index_name = "demo_index"
    req.index_schema.field_configs.extend([
        admin_pb2.FieldConfig(
            field_name="textField",
            field_type=admin_pb2.FIELD_TYPE_STRING_ANALYZED)])
    response = stub.CreateIndex(req)
    print(response)

@click.command()
@click.argument('query_param')
def query(query_param):
    channel = grpc.insecure_channel('localhost:50051')
    stub = search_pb2_grpc.SearchServiceStub(channel)
    req = search_pb2.SearchRequest()
    req.index_name = "demo_index"
    req.count = 10
    req.params.combine_with = search_pb2.COMBINE_WITH_MUST
    param = search_pb2.Param()
    param.query_param.fields.extend(["textField"])
    param.query_param.query = query_param
    req.params.params.extend([param])
    response = stub.Query(req)
    print(response)


@click.group()
def cli():
    return

cli.add_command(index)
cli.add_command(create_index)
cli.add_command(query)

if __name__ =='__main__':
    cli()
