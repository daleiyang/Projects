cc_library(
    name = 'Storage',
    srcs = 'Storage.cc',
    deps = [
        '//Common/Util/Trie:jumpTrie',
        '//Common/ThirdParty/curl:curl',
        '//Common/Util/HttpClient:curlWrapper',
        '//Common/ThirdParty/jsoncpp-0.5.0/src/lib_json:jsoncpp',
    ],
    extra_cppflags = ['-std=c++11'],
)

cc_library(
    name = 'HighlightNewsServingHandle',
    srcs = 'HighlightNewsServingHandle.cc',
    deps = [
        ':Storage',
        '//Robot/Public/Proto:serviceThrift',
    ],
    extra_cppflags = ['-std=c++11'],
)

cc_binary(
    name = 'HighlightNewsServer',
    srcs = 'HighlightNewsServer.cc',
    deps = [
        ':HighlightNewsServingHandle',
        '//Common/ThirdParty/zk_wrapper:zk_wrapper',
        '//Common/ThirdParty/thrift-1.0.0-dev:thriftnb',
        '//Common/ThirdParty/thrift-1.0.0-dev:thrift_c_glib',
        '//Common/ThirdParty/thrift-1.0.0-dev:thriftz',
        '//Common/ThirdParty/thrift-1.0.0-dev:thrift',
        '//Common/ThirdParty/libevent-2.0.11-stable:event',
    ],
    extra_cppflags = ['-std=c++11'],
)

