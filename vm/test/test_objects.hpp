#include "objects.hpp"
#include "vm.hpp"

#include <cxxtest/TestSuite.h>

using namespace rubinius;

class TestObjects : public CxxTest::TestSuite {
  public:

  void test_object() {
    VM vm(1024);
    TS_ASSERT_EQUALS(vm.globals.object->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(vm.globals.object->superclass, Qnil);
  }

  void test_class() {
    VM vm(1024);
    Class *cls;

    cls = (Class*)vm.globals.klass;
    TS_ASSERT_EQUALS(cls->class_object(), cls);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.module);
  }

  void test_metaclass_is_completely_setup() {
    VM vm(1024);
    Class *cls;
    MetaClass *meta;

    cls = (Class*)vm.globals.klass;
    meta = (MetaClass*)cls->klass;
    TS_ASSERT(MetaClass::is_a(vm.globals.object->klass));
    TS_ASSERT(LookupTable::is_a(meta->method_table));
    TS_ASSERT(LookupTable::is_a(meta->constants));
  }

  void test_module() {
    VM vm(1024);
    Class *mod;

    mod = (Class*)vm.globals.module;
    TS_ASSERT_EQUALS(mod->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(mod->superclass, vm.globals.object);
  }

  void test_metaclass() {
    VM vm(1024);
    Class *meta;

    meta = vm.globals.metaclass;

    TS_ASSERT_EQUALS(meta->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(meta->superclass, vm.globals.klass);
    TS_ASSERT_EQUALS(meta->object_type, Object::i2n(MetaclassType));
  }

  void test_tuple() {
    VM vm(1024);
    Class *tup;

    tup = vm.globals.tuple;

    TS_ASSERT_EQUALS(tup->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(tup->superclass, vm.globals.object);
    TS_ASSERT_EQUALS(tup->object_type, Object::i2n(TupleType));
  }

  void test_lookuptable() {
    VM vm(1024);
    Class *lt;

    lt = vm.globals.lookuptable;

    TS_ASSERT_EQUALS(lt->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(lt->superclass, vm.globals.object);
    TS_ASSERT_EQUALS(lt->object_type, Object::i2n(LookupTableType));
  }

  void test_methtbl() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.methtbl;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.lookuptable);

    TS_ASSERT_EQUALS((object_type)cls->object_type->n2i(), MTType);
  }

  void test_symbol() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.lookuptable;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
  }

  void test_array() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.array;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
  }

  void test_bytearray() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.bytearray;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
    TS_ASSERT_EQUALS((object_type)cls->object_type->n2i(), ByteArrayType);
  }

  void test_string() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.string;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
    TS_ASSERT_EQUALS((object_type)cls->object_type->n2i(), StringType);
  }

  void test_symtbl() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.symtbl;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
  }

  void test_cmethod() {
    VM vm(1024);
    Class *cls;

    cls = vm.globals.cmethod;

    TS_ASSERT_EQUALS(cls->class_object(), vm.globals.klass);
    TS_ASSERT_EQUALS(cls->superclass, vm.globals.object);
  }
};
