#include "Type.h"

// to avoid compiler confusion, python.hpp must be include before Halide headers
#include <boost/python.hpp>

#include "../../src/Type.h"
#include "../../src/Expr.h"

#include <string>
#include <boost/format.hpp>

namespace h = Halide;


std::string type_code_to_string(const h::Type &t)
{
    std::string code_string = "unknown";
    switch(t.code)
    {
    case h::Type::UInt:
        code_string = "UInt";
        break;

    case h::Type::Int:
        code_string = "Int";
        break;

    case h::Type::Float:
        code_string = "Float";
        break;

    case h::Type::Handle:
        code_string = "Handle";
        break;

    default:
        code_string = "unknown";
    }

    return code_string;
}

std::string type_repr(const h::Type &t)
{
    auto message_format = boost::format("<halide.Type code '%s' with %i bits and width %i>");


    return boost::str(message_format % type_code_to_string(t) % t.bits % t.width);
}

void defineType()
{

    using Halide::Type;
    namespace p = boost::python;
    

    p::class_<Type>("Type", p::no_init)
            .def_readonly("bits", &Type::bits,
                          "The number of bits of precision of a single scalar value of this type.")
            .def("bytes", &Type::bytes,
                 "The number of bytes required to store a single scalar value of this type. Ignores vector width.")
            .def_readonly("width", &Type::width,
                          "How many elements (if a vector type). Should be 1 for scalar types.")
            .def("is_bool", &Type::is_bool, p::arg("self"),
                 "Is this type boolean (represented as UInt(1))?")
            .def("is_vector", &Type::is_vector, p::arg("self"),
                 "Is this type a vector type? (width > 1)")
            .def("is_scalar", &Type::is_scalar, p::arg("self"),
                 "Is this type a scalar type? (width == 1)")
            .def("is_float", &Type::is_float, p::arg("self"),
                 "Is this type a floating point type (float or double).")
            .def("is_int", &Type::is_int, p::arg("self"),
                 "Is this type a signed integer type?")
            .def("is_uint", &Type::is_uint, p::arg("self"),
                 "Is this type an unsigned integer type?")
            .def("is_handle", &Type::is_handle, p::arg("self"),
                 "Is this type an opaque handle type (void *)")
            .def(p::self == p::self)
            .def(p::self != p::self)
            .def("vector_of", &Type::vector_of, p::args("self", "w"),
                 "Produce a vector of this type, with 'width' elements")
            .def("element_of", &Type::element_of, p::arg("self"),
                 "Produce the type of a single element of this vector type")
            .def("can_represent", &Type::can_represent, p::arg("other"),
                 "Can this type represent all values of another type?")
            .def("imax", &Type::imax, p::arg("self"),
                 "Return an integer which is the maximum value of this type.")
            .def("max", &Type::max, p::arg("self"),
                 "Return an expression which is the maximum value of this type")
            .def("imin", &Type::imin, p::arg("self"),
                 "Return an integer which is the minimum value of this type")
            .def("min", &Type::min, p::arg("self"),
                 "Return an expression which is the minimum value of this type")
            .def("__repr__", &type_repr, p::arg("self"),
                 "Return a string containing a printable representation of a Type object.")
            ;

    p::def("Int", h::Int,
           (p::arg("bits"), p::arg("width")=1),
           "Constructing an signed integer type");

    p::def("UInt", h::UInt,
           (p::arg("bits"), p::arg("width")=1),
           "Constructing an unsigned integer type");

    p::def("Float", h::Float,
           (p::arg("bits"), p::arg("width")=1),
           "Constructing a floating-point type");

    p::def("Bool", h::Bool,
           (p::arg("width")=1),
           "Construct a boolean type");

    return;
}


