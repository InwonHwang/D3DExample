#pragma once

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <fbxsdk.h>

template<typename T>
using sp = boost::shared_ptr<T>;

template<typename T>
using wp = boost::weak_ptr<T>;