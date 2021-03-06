// Author: Christoph Lassner.
#ifdef CAFFE_FEATURE_EXTRACTION_ENABLED
#include <fertilized/fertilized.h>

#include <vector>

#include "boost/test/unit_test.hpp"

#include "setup.h"

using namespace fertilized;

BOOST_AUTO_TEST_SUITE(Correctness_DNNFeatureExtractor);

BOOST_AUTO_TEST_CASE(Correctness_Constructor) {
        auto soil = Soil<>();
        auto feature_extractor = soil.DNNFeatureExtractor("netspec.prototxt",
                                                          "netweights.prototxt",
                                                          "_layer_1",
                                                          true,
                                                          0); // Use CPU!
        // Create a dummy 'image'.
        Array<float, 3, 3> testimage = allocate(10, 10, 3);
        testimage.deep() = 0.f;
        std::vector<Array<float, 3, 3> > images;
        images.push_back(testimage);
        auto extraction_result = feature_extractor->extract(images, false);
        std::cout << extraction_result[0][0][0][0];
        CHECK_CLOSE_(extraction_result[0][0][0][0], 0.475574f);

        // Failure checks.
        // Negative device id.
        BOOST_CHECK_THROW(soil.DNNFeatureExtractor("netspec.prototxt",
                                                   "netweights.prototxt",
                                                   "_layer_1",
                                                   true,
                                                   -1),
                          Fertilized_Exception);
        // Wrong model file.
        BOOST_CHECK_THROW(soil.DNNFeatureExtractor("asdf",
                                                   "netweights.prototxt",
                                                   "_layer_1",
                                                   true,
                                                   0),
                          Fertilized_Exception);
        // Wrong weights file.
        BOOST_CHECK_THROW(soil.DNNFeatureExtractor("netspec.prototxt",
                                                   "asdf",
                                                   "_layer_1",
                                                   true,
                                                   0),
                          Fertilized_Exception);
        // Wrong layer name.
        BOOST_CHECK_THROW(soil.DNNFeatureExtractor("netspec.prototxt",
                                                   "netweights.prototxt",
                                                   "asdf",
                                                   true,
                                                   0),
                          Fertilized_Exception);
        // Wrong mean file.
        BOOST_CHECK_THROW(soil.DNNFeatureExtractor("netspec.prototxt",
                                                   "netweights.prototxt",
                                                   "_layer_1",
                                                   true,
                                                   0,
                                                   "asdf"),
                          Fertilized_Exception);
};

#ifndef CAFFE_CPU_ONLY
BOOST_AUTO_TEST_CASE(Correctness_CPU_eq_GPU) {
        auto soil = Soil<>();
        auto feature_extractor_cpu = soil.DNNFeatureExtractor("netspec.prototxt",
                                                              "netweights.prototxt",
                                                              "_layer_1",
                                                              true);
        auto feature_extractor_gpu = soil.DNNFeatureExtractor("netspec.prototxt",
                                                              "netweights.prototxt",
                                                              "_layer_1",
                                                              false);
        Array<float, 3, 3> testimage = allocate(10, 10, 3);
        testimage.deep() = 200.f;
        std::vector<Array<float, 3, 3> > images;
        images.push_back(testimage);
        auto extraction_result_cpu = feature_extractor_cpu->extract(images, false);
        auto extraction_result_gpu = feature_extractor_gpu->extract(images, false);
        auto diff = extraction_result_cpu - extraction_result_gpu;
        BOOST_CHECK(all(equal(extraction_result_cpu, extraction_result_gpu)));
};
#endif

BOOST_AUTO_TEST_SUITE_END();
#endif // CAFFE_FEATURE_EXTRACTION_ENABLED
