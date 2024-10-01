cc_library(
    name = 'tfCWrapper',
    srcs = 'tfCWrapper.cc',
    extra_linkflags = [
            '-ltensorflow',
            '-ltensorflow_framework',
            '-LCommon/ThirdParty/tf_c_api/lib64_release',
        ],
)

cc_library(
    name = 'TmallShoppingContentPlugin',
    srcs = [
        'TmallShoppingContentPlugin.cc',
    ],
    deps = [
        '//Common/Base:Base',
        '//Robot/Public/Proto:serviceThrift',
        '//Common/ThirdParty/jsoncpp-0.5.0/src/lib_json:jsoncpp',
        '//Common/Util/HttpClient:curlWrapperHTTPS',
        '//Common/ThirdParty/cppjieba.5.0.0:Jieba',
        '//frame_semantic_parser_new:parser_new_without_encode',
        '//Projects/Content/lib/trie_baike:baike_extrator',
    ],
    link_all_symbols = True,
    extra_cppflags = ['-fno-omit-frame-pointer', '-Wl,--eh-frame-hdr','-std=c++11'],
)

cc_library(
    name = 'Storage',
    srcs = 'Storage.cc',
    deps = [
        ':tfCWrapper',
        ':TmallShoppingContentPlugin',
        '//Common/Base:Base',
        '//Common/Util/Trie:jumpTrie',
        '//Common/Util/File:simpleLineReader',
        '//Common/Util/Encode:encodeUtil',
        '//Common/Util/Log:TrioLog',
        '//Common/ThirdParty/jsoncpp-0.5.0/src/lib_json:jsoncpp',
        '//Common/ThirdParty/hiredis:hiredis',
        '//frame_semantic_parser_new:parser_new_without_encode',
    ],
    extra_cppflags = ['-std=c++11'],
)

cc_library(
	name = 'ProductServingHandler',
	srcs = 'ProductServingHandler.cc',
	deps = [
            ':Storage',
            '//Robot/Public/Proto:serviceThrift',
            '//Common/Util/Log:TrioLog',
	],
	extra_cppflags = ['-std=c++11'],
)

cc_binary(
	name = 'ProductServer',
	srcs = 'ProductServer.cc',
	deps = [
	    ':ProductServingHandler',
            '//Common/ThirdParty/zk_wrapper:zk_wrapper',
	    '//Common/ThirdParty/thrift-1.0.0-dev:thriftnb',
            '//Common/ThirdParty/thrift-1.0.0-dev:thrift_c_glib',
            '//Common/ThirdParty/thrift-1.0.0-dev:thriftz',
            '//Common/ThirdParty/thrift-1.0.0-dev:thrift',
            '//Common/ThirdParty/libevent-2.0.11-stable:event',
	],
	extra_cppflags = ['-std=c++11'],
        extra_linkflags = [
            #for tfCWrapper
            '-ltensorflow',
            '-ltensorflow_framework',
            '-LCommon/ThirdParty/tf_c_api/lib64_release',
            #for curl
            '-lcurl',
            '-LCommon/ThirdParty/curl-7.53.1/lib64_release',
        ],
)

