// Copyright (C) 2014 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef FOONATHAN_STRING_ID_ERROR_HPP_INCLUDED
#define FOONATHAN_STRING_ID_ERROR_HPP_INCLUDED

#include <exception>
#include <string>

#include "hash.hpp"

namespace foonathan { namespace string_id
{
    /// \brief The base class for all custom exception classes of this library.
    class error : public std::exception
    {
    protected:
        error() noexcept = default;
    };
    
    /// \brief The type of the collision handler.
    /// \detail It will be called when a string hashing results in a collision giving it the two strings collided.
    /// The default handler throws an exception of type \ref collision_error.
    using collision_handler = void(*)(hash_type hash, const char *a, const char *b);
    
    /// \brief Exchanges the \ref collision_handler.
    /// \detail This function is thread safe.
    collision_handler set_collision_handler(collision_handler h);
    
    /// \brief Returns the current \ref collision_handler.
    collision_handler get_collision_handler();
    
    /// \brief The exception class thrown by the default \ref collision_handler.
    class collision_error : public error
    {
    public:
        //=== constructor/destructor ===//
        /// \brief Creates a new exception, same parameter as \ref collision_handler.
        collision_error(hash_type hash, const char *a, const char *b)
        : a_(a), b_(b), hash_(hash) {}
        
        ~collision_error() noexcept override = default;
        
        //=== accessors ===//
        const char* what() const noexcept override;
        
        /// @{
        /// \brief Returns one of the two strings that colllided.
        const char* first_string() const noexcept
        {
            return a_.c_str();
        }
        
        const char* second_string() const noexcept
        {
            return b_.c_str();
        }
        /// @}
        
        /// \brief Returns the hash code of the collided strings.
        hash_type hash_code() const noexcept
        {
            return hash_;
        }
        
    private:
        std::string a_, b_;
        hash_type hash_;
    };
    
    /// \brief The type of the generator error handler.
    /// \detail It will be called when a generator would generate a \ref string_id that already was generated.
    /// The generator will try again until the handler returns \c false in which case it just returns the old \c string_id.
    /// It passes the number of tries, the name of the generator and the hash and string of the generated \c string_id.<br>
    /// The default handler allows 8 tries and then throws an exception of type \ref generation_error.
    using generation_error_handler = bool(*)(std::size_t no, const char *generator_name,
                                             hash_type hash, const char *str);
    
    /// \brief Exchanges the \ref generation_error_handler.
    /// \detail This function is thread safe.
    generation_error_handler set_generation_error_handler(generation_error_handler h);
    
    /// \brief Returns the current \ref generation_error_handler.
    generation_error_handler get_generation_error_handler();
    
    /// \brief The exception class thrown by the default \ref generation_error_handler.
    class generation_error : public error
    {
    public:
        //=== constructor/destructor ===//
        /// \brief Creates it by giving it the name of the generator.
        generation_error(const char *generator_name)
        : name_(generator_name) {}
        
        ~generation_error() noexcept override = default;
        
        //=== accessors ===//
        const char* what() const noexcept override;
        
        /// \brief Returns the name of the generator.
        const char* generator_name() const noexcept
        {
            return name_.c_str();
        }
        
    private:
        std::string name_;
    };
}} // namespace foonathan::string_id

#endif // FOONATHAN_STRING_ID_ERROR_HPP_INCLUDED
