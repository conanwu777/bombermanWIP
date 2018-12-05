//
// Created by Hugo JEANNIN on 5/23/17.
//

#include "../inc/AnimationManager.hpp"

AnimationManager::AnimationManager()
{
    std::cerr << "AnimationManager() constructor called." << std::endl;
}

AnimationManager::~AnimationManager()
{
    std::cerr << "AnimationManager() destructor called." << std::endl;
}

void AnimationManager::init()
{
    using namespace glm;

    loadMinimal();
}

void AnimationManager::loadMinimal()
{
    std::cerr << "Loading Minimal animation." << std::endl;
    clearEverything();
    sceneMinimal();
    animMinimal();
    animMinimal2();
}

void AnimationManager::loadTruc()
{
    std::cerr << "Loading Truc animation." << std::endl;
    clearEverything();
    sceneTruc();
}

void
AnimationManager::buildCustomModelAttrib(int amount, std::string name, int model_id, GLuint texture_id, glm::mat4 init_matrix)
{
    int iter = 0;
    while (iter < amount)
    {
        v_model_attrib.push_back({
                                         true,
                                         -1,
                                         -1,
                                         name,
                                         model_id,
                                         texture_id,
                                         init_matrix,
                                         init_matrix,
                                         glm::mat4(1.0f),
                                         glm::mat4(1.0f),
                                         glm::mat4(1.0f),
                                         glm::mat4(1.0f)
                                 });
        iter++;
    }
}

void AnimationManager::addEmptyTS(int amount)
{
    int iter = 0;
    while (iter < amount)
    {
        v_transf_serie.push_back({
                                         false,
                                         0.0f,
                                         0.0f,
                                         0.0f,
                                         0.0f
                                 });
        iter++;
    }
}

void AnimationManager::createNewTransformation(double duration, int type, glm::mat4 matrix, std::string name)
{
    t_transf *pouet = new t_transf;

    pouet->duration = duration;
    pouet->type = type;
    pouet->matrix = matrix;
    pouet->name = name;

    origin_transf_ptr.push_back(pouet);
}

void AnimationManager::buildTS(t_transf_serie *transf_serie, int size, ...)
{
    /// truc du futur qui te prend des argument a l' infini.
    va_list args;
    va_start(args, size);
    for (int iter_args = 0; iter_args < size; ++iter_args)
        transf_serie->serie.push_back(origin_transf_ptr[va_arg(args, int)]);
    va_end(args);
}

void AnimationManager::setTSIndividualStartTimings(t_transf_serie *transf_serie, int size, ...)
{
    /// truc du futur qui te prend des argument a l' infini.
    va_list args;
    va_start(args, size);
    for (int iter_args = 0; iter_args < size; ++iter_args)
        transf_serie->start_timing.push_back(va_arg(args, double));
    va_end(args);
}

void AnimationManager::linkTS(t_transf_serie *transf_serie, t_model_attrib *model_attrib)
{
    transf_serie->models.push_back(model_attrib);
}

void AnimationManager::linkTS(int TS_ID, int ma_ID)
{
    v_transf_serie[TS_ID].models.push_back(&v_model_attrib[ma_ID]);
}

void AnimationManager::setTSGlobalDuration(t_transf_serie *transf_serie, double end_time, double delay)
{
    transf_serie->serie_start_timer = delay;
    transf_serie->serie_end_timer = end_time;
}

void AnimationManager::applyEndFormation(t_transf_serie &transf_serie)
{
    for (auto &model : transf_serie.models)
    {
        model->init_mat = model->end_formation;
        model->matrix = model->init_mat;
        model->pos_id = model->end_pos_id;
    }
}

void AnimationManager::activateEnsemble(std::vector<t_transf_serie *> &ensemble)
{
    double current_time = glfwGetTime();
    for (auto &ts : ensemble)
    {
        ts->is_active = true;
        ts->serie_start_time = current_time + ts->serie_start_timer;
        ts->serie_end_time = ts->serie_start_time + ts->serie_end_timer;
    }
}

void AnimationManager::stopAllAnimation()
{
    /// Stop everything
    for (auto &iter_v_ma : v_model_attrib)
    {
        iter_v_ma.trans_mat = glm::mat4(1.0f);
        iter_v_ma.rot_mat = glm::mat4(1.0f);
        iter_v_ma.scale_mat = glm::mat4(1.0f);
        iter_v_ma.matrix = iter_v_ma.init_mat;
    }
    for (auto &iter_v_ts : v_transf_serie)
    {
        iter_v_ts.is_active = false;
        iter_v_ts.serie_start_time = 0.0f;
        iter_v_ts.serie_end_time = 0.0f;
    }
}

void AnimationManager::stopSerie(t_transf_serie &transf_serie)
{
    transf_serie.is_active = false;
    transf_serie.serie_start_time = 0.0f; /// more security than anything
    transf_serie.serie_end_time = 0.0f;
    for (auto &tsm : transf_serie.models)
    {
        tsm->trans_mat = glm::mat4(1.0f);
        tsm->rot_mat = glm::mat4(1.0f);
        tsm->scale_mat = glm::mat4(1.0f);
        tsm->matrix = tsm->init_mat;
    }
}

void AnimationManager::clearTSsModels(int start, int end)
{
    for (int iter_ts = start; iter_ts <= end; ++iter_ts)
        v_transf_serie[iter_ts].models.clear();
}

void AnimationManager::update()
{
    /// Apply init_matrix even if not active.
    for (auto &model : v_model_attrib)
    {
        model.matrix = glm::mat4(1.0f);
        model.matrix *= model.init_mat;
    }

    if (ANIMATION_VERBOSE)
        std::cerr << "{ ";

    for (auto &iter_ts : v_transf_serie)
    {
        if (iter_ts.is_active)
        {
            /// If current time is in range.
            if (glfwGetTime() >= iter_ts.serie_start_time &&
                glfwGetTime() < iter_ts.serie_end_time)
            {
                for (int iter_t = 0; iter_t < iter_ts.serie.size(); ++iter_t)
                {
                    /// If transf should start.
                    if (glfwGetTime() >=
                        iter_ts.serie_start_time +
                        iter_ts.start_timing[iter_t]
                        && glfwGetTime() <
                           iter_ts.serie_start_time +
                           iter_ts.start_timing[iter_t] +
                           iter_ts.serie[iter_t]->duration)
                    {
                        if (ANIMATION_VERBOSE)
                        {
                            std::cerr << "TIME: " << iter_ts.serie_start_time +
                                                     iter_ts.start_timing[iter_t] +
                                                     iter_ts.serie[iter_t]->duration
                                      << " > " << glfwGetTime() << " > " <<
                                      iter_ts.serie_start_time +
                                      iter_ts.start_timing[iter_t] << " ";
                        }
                        for (int iter_m = 0; iter_m < iter_ts.models.size(); ++iter_m)
                        {
                            if (iter_ts.serie[iter_t]->type == TRANS)
                                iter_ts.models[iter_m]->trans_mat *= iter_ts.serie[iter_t]->matrix;
                            if (iter_ts.serie[iter_t]->type == ROT)
                                iter_ts.models[iter_m]->rot_mat *= iter_ts.serie[iter_t]->matrix;
                            if (iter_ts.serie[iter_t]->type == SCALE)
                                iter_ts.models[iter_m]->scale_mat *= iter_ts.serie[iter_t]->matrix;
                        }
                    }
                    for (auto &model : iter_ts.models)
                    {
                        if (ANIMATION_VERBOSE)
                            std::cerr << "[" << model->name << " -> " << iter_ts.serie[iter_t]->name << "] ";
                        /// combine transformation
                        model->matrix = glm::mat4(1.0f);
                        model->matrix *= model->init_mat;
                        model->matrix *= model->trans_mat;
                        model->matrix *= model->rot_mat;
                        model->matrix *= model->scale_mat;
                    }
                }
            }
            if (glfwGetTime() >= iter_ts.serie_end_time)
            {
                stopSerie(iter_ts);
                applyEndFormation(iter_ts);
            }
        }
    }
    if (ANIMATION_VERBOSE)
        std::cerr << "}" << std::endl;
}

void AnimationManager::clearEverything()
{
    stopAllAnimation();
    origin_transf_ptr.clear();
    v_transf_serie.clear();
    v_model_attrib.clear();
}

void AnimationManager::sceneMinimal()
{
    using namespace glm;
    /// All these step are needed for a very basic animation.

    /// Setup OpenGL model_id, texture_id, and put all requiered Mat4 to identity, default init matrix is identity.
    buildCustomModelAttrib(1, "0_red vase", 0, CHECKER_TEXTURE);
    buildCustomModelAttrib(1, "1_small crate", 0, CHECKER_TEXTURE,
                           translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.0f)));
    buildCustomModelAttrib(1, "2_big crate", 0, CHECKER_TEXTURE,
                           translate(mat4(1.0f), vec3(0.0f, -10.0f, 0.0f)));
    buildCustomModelAttrib(1, "3_stone", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f)));
    buildCustomModelAttrib(1, "4_tree", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(-10.0f, -10.0f, -10.0f)));
    buildCustomModelAttrib(1, "5_minecraft_cube", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(2.0f, 3.0f, 2.0f)));

    /// matrix placement and transf at start.
    v_model_attrib[2].init_mat *= scale(mat4(1.0f), vec3(10.0f, 10.0f, 10.0f));
}

void AnimationManager::animMinimal()
{
    using namespace glm;

    /// All these step are needed for a very basic animation.

    /// Set how many Transf_serie you will need in TOTAL. Here max 4.
    addEmptyTS(4);

    /// Add a transformation to origin_transf_ptr (a vector that keep information about a transformation) each transformation get their ID in order 0, 1, 2 ...
    createNewTransformation(5.0f, ROT, rotate(mat4(1.0f), 2.5f / TPS, vec3(1.0f, 1.0f, 0.0f)), "0_rotate_XY");
    createNewTransformation(5.0f, ROT, rotate(mat4(1.0f), -2.5f / TPS, vec3(1.0f, 1.0f, 0.0f)), "1_rotate_XY_reverse");
    createNewTransformation(3.0f, TRANS, translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.5f / TPS)), "2_move_back");
    createNewTransformation(3.0f, TRANS, translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f / TPS)), "3_move_front");
    createNewTransformation(3.0f, ROT, rotate(mat4(1.0f), 2.5f / TPS, vec3(1.0f, 1.0f, 0.0f)), "4_rotate_XY");
    createNewTransformation(3.0f, ROT, rotate(mat4(1.0f), -2.5f / TPS, vec3(1.0f, 1.0f, 0.0f)), "5_rotate_XY_reverse");
    createNewTransformation(2.0f, TRANS, translate(mat4(1.0f), vec3(0.0f, 1.0f / TPS, 0.0f)), "6_move_up");
    createNewTransformation(2.0f, TRANS, translate(mat4(1.0f), vec3(0.0f, -1.0f / TPS, 0.0f)), "7_move_down");
    createNewTransformation(2.0f, ROT, rotate(mat4(1.0f), 0.014f, vec3(1.0f, 0.0f, 0.0f)), "8_rotate_x");

    /// 1st line -> add 3 transformation, number 0, 1 and 6 to transformation serie 0.
    buildTS(&v_transf_serie[0], 3, 0, 1, 6);
    buildTS(&v_transf_serie[1], 2, 2, 3);
    buildTS(&v_transf_serie[2], 2, 4, 5);
    buildTS(&v_transf_serie[3], 2, 7, 8);

    /// for each transformation set the start time. T0 start @ 0sec, T1 start @ 5sec, T6 start @ 10sec.
    setTSIndividualStartTimings(&v_transf_serie[0], 3, 0.0f, 5.0f, 10.0f);
    setTSIndividualStartTimings(&v_transf_serie[1], 2, 0.0f, 3.0f);
    setTSIndividualStartTimings(&v_transf_serie[2], 2, 0.0f, 3.0f);
    setTSIndividualStartTimings(&v_transf_serie[3], 2, 0.0f, 0.0f);

    /// How long will the TS last? global transformation will last 12 second and start with a 0.5 second delay.
    setTSGlobalDuration(&v_transf_serie[0], 12.0f, 0.5f);
    setTSGlobalDuration(&v_transf_serie[1], 6.0f, 0.5f);
    setTSGlobalDuration(&v_transf_serie[2], 6.0f, 0.5f);
    setTSGlobalDuration(&v_transf_serie[3], 2.0f, 0.0f);
}

void AnimationManager::animMinimal2()
{
    using namespace glm;

    addEmptyTS(1);
    createNewTransformation(2.0f, ROT, rotate(mat4(1.0f), 0.42f, vec3(1.0f, 0.0f, 0.0f)), "9_rotate_x_FAST");
    buildTS(&v_transf_serie[4], 1, 9);
    setTSIndividualStartTimings(&v_transf_serie[4], 1, 0.0f);
    setTSGlobalDuration(&v_transf_serie[4], 2.0f, 0.0f);
}

void AnimationManager::applyMinimalStep1()
{
    /// clear any possible models linked in TS.
    clearTSsModels(0, 2);

    /// set on which model_attrib to apply the transformation serie.
    linkTS(&v_transf_serie[0], &v_model_attrib[0]);
    linkTS(&v_transf_serie[1], &v_model_attrib[1]);
    linkTS(&v_transf_serie[2], &v_model_attrib[1]);

    /// Set end formation is like setting the next keyframe. Where matrix should be if there was no precision pb.
    /// Default behaviour in buildCustomModelAttrib is setting end_formation = init_matrix.
    v_transf_serie[0].models[0]->end_formation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));

    ensemble1.clear();
    /// add each TS (Transformation Serie) into your E (ensemble of TS). this ensemble can be launch pressing [SPACE]
    ensemble1.push_back(&v_transf_serie[0]);
    ensemble1.push_back(&v_transf_serie[1]);
    ensemble1.push_back(&v_transf_serie[2]);

    activateEnsemble(ensemble1);
}

void AnimationManager::applyMinimalStep2()
{
    v_transf_serie[3].models.clear();
    v_transf_serie[4].models.clear();

    linkTS(&v_transf_serie[3], &v_model_attrib[0]);
    linkTS(&v_transf_serie[4], &v_model_attrib[3]);

    v_transf_serie[3].models[0]->end_formation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    v_transf_serie[4].models[0]->end_formation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    ensemble1.clear();

    ensemble1.push_back(&v_transf_serie[3]);
    ensemble1.push_back(&v_transf_serie[4]);

    activateEnsemble(ensemble1);
}

void AnimationManager::sceneTruc()
{
    using namespace glm;
    /// All these step are needed for a very basic animation.

    /// Setup OpenGL model_id, texture_id, and put all requiered Mat4 to identity, default init matrix is identity.
    buildCustomModelAttrib(1, "0_stone", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(2.0f, 0.0f, 0.0f)));
    buildCustomModelAttrib(1, "1_stone", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)));
    buildCustomModelAttrib(1, "2_stone", 0, CHECKER_TEXTURE, translate(mat4(1.0f), vec3(-2.0f, 0.0f, 0.0f)));

    /// matrix placement and transf at start.
    v_model_attrib[0].init_mat *= scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    v_model_attrib[1].init_mat *= scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
    v_model_attrib[2].init_mat *= scale(mat4(1.0f), vec3(3.0f, 3.0f, 3.0f));

}

void AnimationManager::printModelAttribPosID()
{
    for (int i = 0; i < 4; ++i)
    {
        std::cerr << "MA: " << i << " pos_id: " << v_model_attrib[i].pos_id << " end_pos_id: "
                  << v_model_attrib[i].end_pos_id << std::endl;
    }
}